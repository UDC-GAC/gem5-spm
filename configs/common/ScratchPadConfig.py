# Copyright (c) 2012-2013 ARM Limited
# All rights reserved
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
#
# Configure the ScratchPad Memory
#

import m5
from m5.objects import *

def config_spm(options, system):
    if (options.scratchpad >= 1):
        addr_start = options.mem_size
        for i in range (1, options.scratchpad+1):
            # Getting attributes from options
            spm_size = getattr(options, "spm_size_" + `i`)
            spm_type = getattr(options, "spm_type_" + `i`)

            #####################
            # Choosing type of SPM:
            # 1.- Inherits from SimpleMemory. Read latency 2ns and 1ns variable. BW=64GBs
            # 2.- Inherits from GDDR5. Modified front end and back end latencies
            ######################
            if (spm_type==1):
                spm = ScratchpadMemory()
            else:
                spm = ScratchpadMemoryDRAM()

            # Needed to choose the range
            spm.range = m5.objects.AddrRange(start = addr_start, size = spm_size)
            # This is a hack. Be careful with it. If there are more spm, they will all be at the end
            addr_start = spm.range.end + 1
            # Just adding the latency: if SimpleMemory
            if (spm_type==1):
                spm.latency = getattr(options, "spm_r_lat_" + `i`)
                spm.latency_var = getattr(options, "spm_rvar_lat_" + `i`)
                spm.latency_write = getattr(options, "spm_w_lat_" + `i`)
                spm.latency_write_var = getattr(options, "spm_wvar_lat_" + `i`)
            else:
                spm.static_frontend_latency = getattr(options, "spm_w_lat_" + `i`)
                spm.static_backend_latency = getattr(options, "spm_r_lat_" + `i`)
                
            # Connecting to master bus 
            spm.port = system.membus.master
            # Adding spm to the system
            setattr(system, "spm_" + `i`, spm)
 
