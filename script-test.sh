#!/bin/bash

################################################
# TEST NO SPM SPM ARCH
./build/X86/gem5.debug --stats-file=test-nospm-spmarch.txt --debug-flags=PseudoInst --debug-file=test-nospm-spmarch.out configs/spm/se.py  --cpu-type=timing --mem-size=4GB --caches -c tests/test-progs/spm/test_dir/testn 

# TEST NO SPM CONVENTIONAL ARCH
./build/X86/gem5.debug --stats-file=test-nospm-convarch.txt --debug-flags=PseudoInst --debug-file=test-nospm-convarch.out configs/example/se.py  --cpu-type=timing --mem-size=4GB --caches -c tests/test-progs/spm/test_dir/testn 

################################################
# TEST With SPM

# Default (5/10)
#./build/X86/gem5.debug --stats-file=test-spm.txt --debug-flags=PseudoInst --debug-file=test-spm.out configs/spm/se.py --cpu-type=timing --mem-size=4GB --caches --scratchpad=1 --spm-size-1=8MB --spm-type-1=1 -c tests/test-progs/spm/test_dir/test

# 2/2ns
./build/X86/gem5.debug --stats-file=test-spm.txt --debug-flags=PseudoInst --debug-file=test-spm.out configs/spm/se.py --cpu-type=timing --mem-size=4GB --caches --scratchpad=1 --spm-size-1=8MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=2ns -c tests/test-progs/spm/test_dir/test
