#!/bin/bash

# No SPM 
#./build/X86/gem5.opt --stats-file=nospm.txt --debug-flags=PseudoInst --debug-file=trace.out  configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/test/testn 

# With SPM
./build/X86/gem5.opt --stats-file=spm.txt --debug-flags=PseudoInst --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=32MB --caches -c tests/test-progs/spm/test/test

# 2 SPM: 1 SP, 2 DP
./build/X86/gem5.opt --stats-file=spmDP.txt --debug-flags=PseudoInst --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad=2 --spm-size-1=32MB --spm-size-2=4MB --spm-type-2=2 --caches -c tests/test-progs/spm/test/test

