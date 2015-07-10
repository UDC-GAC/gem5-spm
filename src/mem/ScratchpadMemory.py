# Copyright (c) 2015. Universidade da Coruña 2015.
#
# Authors: Markos Horro

from m5.params import *
from AbstractMemory import *
from DRAMCtrl import *
from SimpleMemory import *

# BASED ON OWN IMPLEMENTATION (SimpleMemory)

# ORIGINAL Simple memory: 30ns and 0ns. Bandwith=12.8GB/s (DDR3-1600)
class ScratchpadMemory(SimpleMemory):
   cxx_header = "mem/simple_mem.hh"
   port = SlavePort("Slave ports")
   latency_read = Param.Latency('8ns', "Request to response latency")
   latency_read_var = Param.Latency('2ns', "Variable latency when reading")

   bandwidth = Param.MemoryBandwidth('12.8GB/s',
                                     "Combined read and write bandwidth")

# SPM low latency
class ScratchpadMemoryLL(SimpleMemory):
   cxx_header = "mem/simple_mem.hh"
   port = SlavePort("Slave ports")
   latency_read = Param.Latency('4ns', "Request to response latency")
   latency_read_var = Param.Latency('1ns', "Variable latency when reading")

   bandwidth = Param.MemoryBandwidth('20GB/s',
                                     "Combined read and write bandwidth")


# BASED ON DRAMCtrl (1300MHz) IMPLEMENTATION.
# A single GDDR5 x64 interface, with
# default timings based on a GDDR5-4000 1 Gbit part (SK Hynix
# H5GQ1H24AFR) in a 2x32 configuration.
class ScratchpadMemoryDRAM(GDDR5_4000_x64):
    # size of device
    device_size = '8MB'
