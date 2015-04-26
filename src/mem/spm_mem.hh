/*
 * Copyright (c) 2015. Markos Horro
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
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

/**
 * @file
 * ScratchpadMemory declaration, based on SimpleMemory
 */

#ifndef __SCRATCHPAD_MEMORY_HH__
#define __SCRATCHPAD_MEMORY_HH__

#include "mem/abstract_mem.hh"
#include "mem/port.hh"
#include "params/ScratchpadMemory.hh"

/**
 * The scratchpad is a basic double-ported memory controller with
 * configurable size and latencies
 *
 * @sa  \ref gem5MemorySystem "gem5 Memory System"
 */
class ScratchpadMemory : public AbstractMemory
{

  private:

    /**
     * Definition ScratchpadSlavePort
     */
    class MemoryPort : public SlavePort
    {

      private:

        ScratchpadMemory& memory;

      public:

        MemoryPort(const std::string& _name, ScratchpadMemory& _memory);

      protected:

        Tick recvAtomic(PacketPtr pkt);

        void recvFunctional(PacketPtr pkt);

        bool recvTimingReq(PacketPtr pkt);

        void recvRespRetry();

        AddrRangeList getAddrRanges() const;

    };

    MemoryPort port;

    // ScratchpadMasterPort master_port; // connect with main memory

    /**
     * Latency from that a request is accepted until the response is
     * ready to be sent.
     */
    const Tick latency_read;

    /**
     * Latency from that a request is sent until the response is
     * received.
     */
    //const Tick latency_write;

    /**
     * Fudge factor added to the read latency
     */
    const Tick latency_read_var;

    /**
     * Fudge factor added to the write latency
     */
    //const Tick latency_write_var;

    /**
     * Detemine the writting latency
     *
     * @return the latency seen by the packet requested
     */
    Tick getLatencyWrite() const;

    /**
     * Detemine the reading latency
     *
     * @return the latency seen by the current packet
     */
    Tick getLatencyRead() const;

  public:

    ScratchpadMemory(const ScratchpadMemoryParams *p);

    BaseSlavePort& getSlavePort(const std::string& if_name,
                                PortID idx = InvalidPortID);

    void init();

  protected:

    Tick recvAtomic(PacketPtr pkt);

    // void recvFunctional(PacketPtr pkt);

    // bool recvTimingReq(PacketPtr pkt);

    void recvRespRetry();

};

#endif //__SCRATCHPAD_MEMORY_HH__


