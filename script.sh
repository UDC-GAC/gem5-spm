#!/bin/bash

################################################
# TEST TIME
#./build/X86/gem5.opt --stats-file=testmp.txt --debug-flags=PseudoInst --debug-file=test-tmp.out  configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/test_dir/testmp

################################################
# TEST TIME SPM
#./build/X86/gem5.opt --stats-file=testmpspm.txt --debug-flags=PseudoInst --debug-file=test-tmp-spm-trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=32MB --spm-type-1=2 --caches -c tests/test-progs/spm/test_dir/testmp --checkpoint-dir=m5out -r 1

################################################
# TEST NO SPM 
#./build/X86/gem5.debug --stats-file=nospm.txt --debug-flags=PseudoInst --debug-file=test-nospm-trace.out  configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/test_dir/testn 

################################################
# TEST With SPM

# 5ns
#./build/X86/gem5.debug --stats-file=spm.txt --debug-flags=PseudoInst --debug-file=test-spm-trace.out configs/spm/se.py --cpu-type=timing --mem-size=4GB --scratchpad=1 --spm-size-1=8MB --spm-type-1=1 --spm-lat-1=5ns -c tests/test-progs/spm/test_dir/test

# 1ns
#./build/X86/gem5.debug --stats-file=spm.txt --debug-flags=PseudoInst --debug-file=test-spm-trace.out configs/spm/se.py --cpu-type=timing --mem-size=4GB --scratchpad=1 --spm-size-1=8MB --spm-type-1=1 --spm-lat-1=1ns -c tests/test-progs/spm/test_dir/test

################################################
# @TODO
# 2MM
#./build/X86/gem5.opt --stats-file=spm2mm.txt --debug-flags=PseudoInst --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=32MB --caches -c tests/test-progs/spm/2mm/2mm

# 2mm ORG
#./build/X86/gem5.opt --stats-file=2mm-no-spm.txt --debug-flags=PseudoInst --debug-file=2mm-trace.out configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/2mm/org

# 2mm SPM no cache
#./build/X86/gem5.opt --stats-file=2mm-spm.txt --debug-flags=PseudoInst --debug-file=2mm-spm-trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=128MB --spm-type-1=2 -c tests/test-progs/spm/2mm/spm

# 2mm SPM cache
#./build/X86/gem5.opt --stats-file=2mm-spm.txt --debug-flags=PseudoInst --debug-file=2mm-spm-trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=128MB --spm-type-1=2 --caches -c tests/test-progs/spm/2mm/spm

################################################
# @TODO
# trmm
# A in main memory, B in SPM
./build/X86/gem5.debug --stats-file=trmm-spm-b-spm.txt configs/spm/se.py --mem-size=512MB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=3ns --spm-wvar-lat-1=1ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

# Both in main memory: see configs/common/Options.py to check the cache configuration
./build/X86/gem5.debug --stats-file=trmm-no-spm.txt configs/example/se.py --mem-size=4GB --caches --cpu-type=timing -c tests/test-progs/spm/trmm/org

# Both in SPM

