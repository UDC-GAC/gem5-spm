#
# Copyright (c) 2015. Universidade da Coruna 2015.
#
# Authors: Markos Horro

from m5.params import *
from AbstractMemory import *
from DRAMCtrl import *
from SimpleMemory import *

# BASED ON OWN IMPLEMENTATION (SimpleMemory)

# ORIGINAL Simple memory: 30ns and 0ns. Bandwith=12.8GB/s (DDR3-1600)
class ScratchpadMemory(SimpleMemory):
   type = 'ScratchpadMemory'
   cxx_header = "mem/spm_mem.hh"
   port = SlavePort("Slave ports")
   latency_write = Param.Latency('10ns', "Write latency in SPM")
   latency_write_var = Param.Latency('0ns', "Write latency in SPM variable")
   # Modeling energy
   energy_read = Param.Float('3.2', "Energy for each reading (pJ)");
   energy_write = Param.Float('4.3', "Energy for each writting (pJ)");
   energy_overhead = Param.Float('0.2', "Overhead energy (pJ)");

   # This parameter is defined as the acceptance rate of request. Not very clear...
   # In the config.ini file is the inverse, e.g. BW=12.8GB/s, bandwidth = 73.0 ps/b
   bandwidth = Param.MemoryBandwidth('64GB/s',
                                     "Combined read and write bandwidth")

# BASED ON DRAMCtrl (1300MHz) IMPLEMENTATION.
# A single GDDR5 x64 interface, with
# default timings based on a GDDR5-4000 1 Gbit part (SK Hynix
# H5GQ1H24AFR) in a 2x32 configuration.
class ScratchpadMemoryDRAM(GDDR5_4000_x64):
    # size of DRAM Chip in Bytes
    device_size = '8MB'

    # pipeline latency of the controller and PHY, split into a
    # frontend part and a backend part, with reads and writes serviced
    # by the queues only seeing the frontend contribution, and reads
    # serviced by the memory seeing the sum of the two
    # Basically: frontend -> reads; backend -> reads and writes
    static_frontend_latency = Param.Latency("10ns", "Static frontend latency")
    static_backend_latency = Param.Latency("5ns", "Static backend latency")

