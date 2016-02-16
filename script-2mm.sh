#!/bin/bash

################################################
# 2MM

# 2/3
./build/X86/gem5.debug --stats-file=2mm-spm.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=3ns --caches --cpu-type=timing -c tests/test-progs/spm/2mm/2mm

# 3/5
./build/X86/gem5.debug --stats-file=2mm-spm.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=3ns --spm-w-lat-1=5ns --caches --cpu-type=timing -c tests/test-progs/spm/2mm/2m

# 2/2
./build/X86/gem5.debug --stats-file=2mm-spm.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=2ns --caches --cpu-type=timing -c tests/test-progs/spm/2mm/2mm

# 2mm conventional arch
./build/X86/gem5.debug --stats-file=2mm-no-spm-convarch.txt configs/example/se.py --mem-size=4GB --caches --cpu-type=timing -ctests/test-progs/spm/2mm/org

# 2mm SPM arch
./build/X86/gem5.debug --stats-file=2mm-no-spm-spmarch.txt configs/spm/se.py --mem-size=4GB --caches --cpu-type=timing -c tests/test-progs/spm/2mm/org
