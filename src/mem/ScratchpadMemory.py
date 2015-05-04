# Copyright (c) 2015. Markos Horro
#
# The license below extends only to copyright in the software and shall
# not be construed as granting a license to any other intellectual
# property including but not limited to intellectual property relating
# to a hardware implementation of the functionality of the software
# licensed hereunder.  You may use the software subject to the license
# terms below provided that you ensure that this notice is replicated
# unmodified and in its entirety in all distributions of the software,
# modified or unmodified, in source code or in binary form.
#
# Copyright (c) 2005-2008 The Regents of The University of Michigan
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors: Markos Horro

from m5.params import *
from AbstractMemory import *
from DRAMCtrl import *

# BASED ON OWN IMPLEMENTATION

#class ScratchpadMemory(AbstractMemory):
#    type = 'ScratchpadMemory'
#    cxx_header = "mem/spm_mem.hh"
#    port = SlavePort("Slave ports")
#    latency_read = Param.Latency('10ns', "Request to response latency")
#    latency_read_var = Param.Latency('5ns', "Variable latency when reading")

#    bandwidth = Param.MemoryBandwidth('12.8GB/s',
#                                      "Combined read and write bandwidth")

    # This memory will be managed by software, should it be on global map?
    # in_addr_map = False

# BASED ON DRAMCtrl IMPLEMENTATION

class ScratchpadMemory(DRAMCtrl):
    # size of device in bytes
    device_size = '8MB'

    # 8x8 configuration, 8 devices each with an 8-bit interface
    device_bus_width = 8

    # DDR3 is a BL8 device
    burst_length = 8

    # Each device has a page (row buffer) size of 1 Kbyte (1K columns x8)
    device_rowbuffer_size = '1kB'

    # 8x8 configuration, so 8 devices
    devices_per_rank = 8

    # Use two ranks
    ranks_per_channel = 1

    # DDR3 has 8 banks in all configurations
    banks_per_rank = 8

    # 1600 MHz
    tCK = '0.625ns'

    # 8 beats across an x64 interface translates to 4 clocks @ 800 MHz
    tBURST = '5ns'

    # DDR3-1600 11-11-11
    tRCD = '10ns'
    tCL = '10ns'
    tRP = '10ns'
    tRAS = '35ns'
    tRRD = '4ns'
    tXAW = '25ns'
    activation_limit = 4
    tRFC = '200ns'

    tWR = '10ns'

    # Greater of 4 CK or 7.5 ns
    tWTR = '7ns'

    # Greater of 4 CK or 7.5 ns
    tRTP = '7ns'

    # Default same rank rd-to-wr bus turnaround to 2 CK, @800 MHz = 2.5 ns
    tRTW = '2ns'

    # Default different rank bus delay to 2 CK, @800 MHz = 2.5 ns
    tCS = '2ns'

    # <=85C, half for >85C
    tREFI = '7us'

    # Current values from datasheet
    IDD0 = '75mA'
    IDD2N = '50mA'
    IDD3N = '57mA'
    IDD4W = '165mA'
    IDD4R = '187mA'
    IDD5 = '220mA'
    VDD = '1.5V'
