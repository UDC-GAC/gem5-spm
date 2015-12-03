/*
 * Copyright (c) 2015. Markos Horro
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Markos Horro
 *
 */

#include "base/random.hh"
#include "mem/spm_mem.hh"
#include "debug/Drain.hh"
#include "sim/system.hh"

using namespace std;

void
ScratchpadMemory::regStats()
{
    using namespace Stats;

    AbstractMemory::regStats();

    System *system = (AbstractMemory::system());

    readEnergy
        .name(name() + ".energy_read")
        .desc("Total energy reading (pJ)")
        .precision(0)
        .prereq(AbstractMemory::numReads)
        .flags(total | nozero | nonan)
        ;
    for (int i = 0; i < system->maxMasters(); i++) {
        readEnergy.subname(i, system->getMasterName(i));
    }

    writeEnergy
        .name(name() + ".energy_write")
        .desc("Total energy writting (pJ)")
        .precision(0)
        .prereq(AbstractMemory::numWrites)
        .flags(total | nozero | nonan)
        ;
    for (int i = 0; i < system->maxMasters(); i++) {
        writeEnergy.subname(i, system->getMasterName(i));
    }

    overheadEnergy
        .name(name() + ".energy_overhead")
        .desc("Other energy (pJ)")
        .precision(0)
        .prereq(AbstractMemory::numOther)
        .flags(total | nozero | nonan)
        ;
    for (int i = 0; i < system->maxMasters(); i++) {
        overheadEnergy.subname(i, system->getMasterName(i));
    }

    totalEnergy
        .name(name() + ".energy_total")
        .desc("Total energy (pJ)")
        .precision(0)
        .prereq(overheadEnergy)
        .flags(total | nozero | nonan)
        ;
    for (int i = 0; i < system->maxMasters(); i++) {
        totalEnergy.subname(i, system->getMasterName(i));
    }

    totalEnergy
        .name(name() + ".energy_average")
        .desc("Average energy (pJ)")
        .precision(0)
        .prereq(totalEnergy)
        .flags(total | nozero | nonan)
        ;
    for (int i = 0; i < system->maxMasters(); i++) {
        averageEnergy.subname(i, system->getMasterName(i));
    }
    
    // Trying to implement a energy model...
    readEnergy = AbstractMemory::numReads * energy_read;
    writeEnergy = AbstractMemory::numWrites * energy_write;
    overheadEnergy = AbstractMemory::numOther * energy_overhead;
    totalEnergy = readEnergy + writeEnergy + overheadEnergy;
    averageEnergy = totalEnergy / 3;
    
}

ScratchpadMemory::ScratchpadMemory(const ScratchpadMemoryParams* p) :
    SimpleMemory(p),
    latency_write(p->latency_write), latency_write_var(p->latency_write_var),
    energy_read(p->energy_read), energy_write(p->energy_write),
    energy_overhead(p->energy_overhead)
{
}

Tick
ScratchpadMemory::recvAtomic(PacketPtr pkt)
{
    access(pkt);
    return pkt->memInhibitAsserted() ? 0 : ((pkt->isRead()) ? getLatency() : getWriteLatency());
}

bool
ScratchpadMemory::recvTimingReq(PacketPtr pkt)
{
    /// @todo temporary hack to deal with memory corruption issues until
    /// 4-phase transactions are complete
    for (int x = 0; x < pendingDelete.size(); x++)
        delete pendingDelete[x];
    pendingDelete.clear();
    
    if (pkt->memInhibitAsserted()) {
        // snooper will supply based on copy of packet
        // still target's responsibility to delete packet
        pendingDelete.push_back(pkt);
        return true;
    }

    // we should never get a new request after committing to retry the
    // current one, the bus violates the rule as it simply sends a
    // retry to the next one waiting on the retry list, so simply
    // ignore it
    if (retryReq)
        return false;

    // if we are busy with a read or write, remember that we have to
    // retry
    if (isBusy) {
        retryReq = true;
        return false;
    }

    // @todo someone should pay for this
    pkt->headerDelay = pkt->payloadDelay = 0;

    // update the release time according to the bandwidth limit, and
    // do so with respect to the time it takes to finish this request
    // rather than long term as it is the short term data rate that is
    // limited for any real memory

    // only look at reads and writes when determining if we are busy,
    // and for how long, as it is not clear what to regulate for the
    // other types of commands
    if (pkt->isRead() || pkt->isWrite()) {
        // calculate an appropriate tick to release to not exceed
        // the bandwidth limit
        Tick duration = pkt->getSize() * bandwidth;

        // only consider ourselves busy if there is any need to wait
        // to avoid extra events being scheduled for (infinitely) fast
        // memories
        if (duration != 0) {
            schedule(releaseEvent, curTick() + duration);
            isBusy = true;
        }
    }

    // go ahead and deal with the packet and put the response in the
    // queue if there is one
    bool needsResponse = pkt->needsResponse();
    recvAtomic(pkt);
    // turn packet around to go back to requester if response expected
    if (needsResponse) {
        // recvAtomic() should already have turned packet into
        // atomic response
        assert(pkt->isResponse());
        // to keep things simple (and in order), we put the packet at
        // the end even if the latency suggests it should be sent
        // before the packet(s) before it.

	// Difference between read/write latencies
	Tick totLat = ((pkt->isRead()) ? getLatency() : 0) + ((pkt->isWrite()) ? getWriteLatency() : 0);
        packetQueue.push_back(DeferredPacket(pkt, curTick() + totLat));
        if (!retryResp && !dequeueEvent.scheduled())
            schedule(dequeueEvent, packetQueue.back().tick);
    } else {
        pendingDelete.push_back(pkt);
    }

    return true;
}

Tick
ScratchpadMemory::getWriteLatency() const
{
    return latency_write +
        (latency_write_var ? random_mt.random<Tick>(0, latency_write_var) : 0);
}

ScratchpadMemory*
ScratchpadMemoryParams::create()
{
    return new ScratchpadMemory(this);
}
