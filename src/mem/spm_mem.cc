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

using namespace std;

ScratchpadMemory::ScratchpadMemory(const ScratchpadMemoryParams* p) :
    AbstractMemory(p),
    port(name() + ".port", *this), 
    latency_read(p->latency_read),
    latency_read_var(p->latency_read_var),bandwidth(p->bandwidth), isBusy(false),
    retryReq(false), retryResp(false),
    releaseEvent(this), dequeueEvent(this), drainManager(NULL)
{
}

void
ScratchpadMemory::init()
{
    AbstractMemory::init();

    // allow unconnected memories as this is used in several ruby
    // systems at the moment
    if (port.isConnected()) {
        port.sendRangeChange();
    }
}

Tick
ScratchpadMemory::getLatencyRead() const
{
    return latency_read +
        (latency_read_var ? random_mt.random<Tick>(0, latency_read_var) : 0);
}

Tick
ScratchpadMemory::recvAtomic(PacketPtr pkt)
{
    access(pkt);
    return pkt->memInhibitAsserted() ? 0 : getLatencyRead();
}

void
ScratchpadMemory::recvFunctional(PacketPtr pkt)
{
    pkt->pushLabel(name());

    functionalAccess(pkt);

    bool done = false;
    auto p = packetQueue.begin();
    // potentially update the packets in our packet queue as well
    while (!done && p != packetQueue.end()) {
        done = pkt->checkFunctional(p->pkt);
        ++p;
    }

    pkt->popLabel();
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
        // before the packet(s) before it
        packetQueue.push_back(DeferredPacket(pkt, curTick() + getLatencyRead()));
        if (!retryResp && !dequeueEvent.scheduled())
            schedule(dequeueEvent, packetQueue.back().tick);
    } else {
        pendingDelete.push_back(pkt);
    }

    return true;
}

void
ScratchpadMemory::release()
{
    assert(isBusy);
    isBusy = false;
    if (retryReq) {
        retryReq = false;
        port.sendRetryReq();
    }
}

void
ScratchpadMemory::dequeue()
{
    assert(!packetQueue.empty());
    DeferredPacket deferred_pkt = packetQueue.front();

    retryResp = !port.sendTimingResp(deferred_pkt.pkt);

    if (!retryResp) {
        packetQueue.pop_front();

        // if the queue is not empty, schedule the next dequeue event,
        // otherwise signal that we are drained if we were asked to do so
        if (!packetQueue.empty()) {
            // if there were packets that got in-between then we
            // already have an event scheduled, so use re-schedule
            reschedule(dequeueEvent,
                       std::max(packetQueue.front().tick, curTick()), true);
        } else if (drainManager) {
            DPRINTF(Drain, "Drainng of ScratchpadMemory complete\n");
            drainManager->signalDrainDone();
            drainManager = NULL;
        }
    }
}

void
ScratchpadMemory::recvRespRetry()
{
    assert(retryResp);

    dequeue();
}

BaseSlavePort &
ScratchpadMemory::getSlavePort(const std::string &if_name, PortID idx)
{
    if (if_name != "port") {
        return MemObject::getSlavePort(if_name, idx);
    } else {
        return port;
    }
}

unsigned int
ScratchpadMemory::drain(DrainManager *dm)
{
    int count = 0;

    // also track our internal queue
    if (!packetQueue.empty()) {
        count += 1;
        drainManager = dm;
        DPRINTF(Drain, "ScratchpadMemory Queue has requests, waiting to drain\n");
     }

    if (count)
        setDrainState(Drainable::Draining);
    else
        setDrainState(Drainable::Drained);
    return count;
}

ScratchpadMemory::MemoryPort::MemoryPort(const std::string& _name,
                                     ScratchpadMemory& _memory)
    : SlavePort(_name, &_memory), memory(_memory)
{ }

AddrRangeList
ScratchpadMemory::MemoryPort::getAddrRanges() const
{
    AddrRangeList ranges;
    ranges.push_back(memory.getAddrRange());
    return ranges;
}

Tick
ScratchpadMemory::MemoryPort::recvAtomic(PacketPtr pkt)
{
    return memory.recvAtomic(pkt);
}

void
ScratchpadMemory::MemoryPort::recvFunctional(PacketPtr pkt)
{
    memory.recvFunctional(pkt);
}

bool
ScratchpadMemory::MemoryPort::recvTimingReq(PacketPtr pkt)
{
    return memory.recvTimingReq(pkt);
}

void
ScratchpadMemory::MemoryPort::recvRespRetry()
{
    memory.recvRespRetry();
}

ScratchpadMemory*
ScratchpadMemoryParams::create()
{
    return new ScratchpadMemory(this);
}

void
ScratchpadMemory::regStats()
{
    using namespace Stats;

    AbstractMemory::regStats();

    for (auto r : ranks) {
        r->regStats();
    }

}
