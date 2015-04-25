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
    slave_port(name() + ".port", *this), 
    latency_read(p->latency_read),
    latency_write(p->latency_write),
    latency_read_var(p->latency_read_var),
    latency_write_var(p->latency_write_var)
{
}

void
ScratchpadMemory::init()
{
    AbstractMemory::init();

    // allow unconnected memories as this is used in several ruby
    // systems at the moment
    if (slave_port.isConnected()) {
        slave_port.sendRangeChange();
    }
}

Tick
ScratchpadMemory::recvAtomic(PacketPtr pkt)
{
    access(pkt);
    return pkt->memInhibitAsserted() ? 0 : getLatencyRead();
}

Tick
ScratchpadMemory::getLatencyRead() const
{
    return latency_read +
        (latency_read_var ? random_mt.random<Tick>(0, latency_read_var) : 0);
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

ScratchpadMemory::ScratchpadSlavePort::ScratchpadSlavePort(const std::string& _name,
                                     ScratchpadMemory& _memory)
    : SlavePort(_name, &_memory), memory(_memory)
{ }

AddrRangeList
ScratchpadMemory::ScratchpadSlavePort::getAddrRanges() const
{
    AddrRangeList ranges;
    ranges.push_back(memory.getAddrRange());
    return ranges;
}

Tick
ScratchpadMemory::ScratchpadSlavePort::recvAtomic(PacketPtr pkt)
{
    return memory.recvAtomic(pkt);
}

// void
// SimpleMemory::MemoryPort::recvFunctional(PacketPtr pkt)
// {
//     memory.recvFunctional(pkt);
// }

// bool
// SimpleMemory::MemoryPort::recvTimingReq(PacketPtr pkt)
// {
//     return memory.recvTimingReq(pkt);
// }

void
ScratchpadMemory::ScratchpadSlavePort::recvRespRetry()
{
    memory.recvRespRetry();
}

ScratchpadMemory*
ScratchpadMemoryParams::create()
{
    return new ScratchpadMemory(this);
}
