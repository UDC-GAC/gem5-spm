#!/bin/bash

# No SPM 
#./build/X86/gem5.opt --stats-file=nospm.txt --debug-flags=PseudoInst --debug-file=trace.out  configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/testn 

# With SPM
./build/X86/gem5.opt --stats-file=spm.txt --debug-flags=PseudoInst --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=32MB --caches -c tests/test-progs/spm/test
