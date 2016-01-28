#!/bin/bash

################################################
# TEST TIME
#./build/X86/gem5.debug --stats-file=testmp.txt --debug-flags=PseudoInst --debug-file=test-tmp.out  configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/test_dir/testmp

################################################
# TEST TIME SPM
#./build/X86/gem5.debug --stats-file=testmpspm.txt --debug-flags=PseudoInst --debug-file=test-tmp-spm-trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=32MB --spm-type-1=2 --caches -c tests/test-progs/spm/test_dir/testmp --checkpoint-dir=m5out -r 1

################################################
# TEST NO SPM 
#./build/X86/gem5.debug --stats-file=test-nospm.txt --debug-flags=PseudoInst --debug-file=test-nospm.out configs/example/se.py  --cpu-type=timing --mem-size=4GB --caches -c tests/test-progs/spm/test_dir/testn 

################################################
# TEST With SPM

# Default (5/10)
#./build/X86/gem5.debug --stats-file=test-spm.txt --debug-flags=PseudoInst --debug-file=test-spm.out configs/spm/se.py --cpu-type=timing --mem-size=4GB --caches --scratchpad=1 --spm-size-1=8MB --spm-type-1=1 -c tests/test-progs/spm/test_dir/test

# 2/3ns
#./build/X86/gem5.debug --stats-file=test-spm.txt --debug-flags=PseudoInst --debug-file=test-spm.out configs/spm/se.py --cpu-type=timing --mem-size=4GB --caches --scratchpad=1 --spm-size-1=8MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=3ns -c tests/test-progs/spm/test_dir/test
