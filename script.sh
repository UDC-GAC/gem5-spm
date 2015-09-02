#!/bin/bash

# No SPM 
#./build/X86/gem5.opt --stats-file=nospm.txt --debug-flags=PseudoInst --debug-file=trace.out  configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/test_dir/testn 

# TEST With SPM
#./build/X86/gem5.opt --stats-file=spm.txt --debug-flags=PseudoInst --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=32MB --caches -c tests/test-progs/spm/test_dir/test

# 2 SPM: 1 SP, 2 DP
#./build/X86/gem5.opt --stats-file=spmDP.txt --debug-flags=PseudoInst --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad=2 --spm-size-1=32MB --spm-size-2=4MB --spm-type-2=2 --caches -c tests/test-progs/spm/test_dir/test

# 2MM
#./build/X86/gem5.opt --stats-file=spm2mm.txt --debug-flags=PseudoInst --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=32MB --caches -c tests/test-progs/spm/2mm/2mm

# 2mm ORG
#./build/X86/gem5.opt --stats-file=2mm-no-spm.txt --debug-flags=PseudoInst --debug-file=2mm-trace.out configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/2mm/org

# 2mm SPM
./build/X86/gem5.opt --stats-file=2mm-spm.txt --debug-flags=PseudoInst --debug-file=2mm-spm-trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=128MB --spm-type-1=2 --caches -c tests/test-progs/spm/2mm/spm
