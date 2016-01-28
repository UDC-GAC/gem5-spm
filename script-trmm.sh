#!/bin/bash

################################################
# @TODO
# trmm
# 1 A in main memory, B in SPM
./build/X86/gem5.debug --stats-file=trmm-spm-1.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=3ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

# 2 A in main memory, B in SPM
./build/X86/gem5.debug --stats-file=trmm-spm-2.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2.5ns --spm-w-lat-1=3.2ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

# 3 A in main memory, B in SPM
./build/X86/gem5.debug --stats-file=trmm-spm-3.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2.2ns --spm-w-lat-1=2.5ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

# 4 Both in main memory: see configs/common/Options.py to check the cache configuration
./build/X86/gem5.debug --stats-file=trmm-no-spm.txt configs/example/se.py --mem-size=4GB --caches --cpu-type=timing -c tests/test-progs/spm/trmm/org
