#!/bin/bash

# No SPM 
#./build/X86/gem5.opt configs/example/se.py --caches -c tests/spm/test 

# With SPM
./build/X86/gem5.opt --debug-flags=Kvm --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad --spm-size=32MB --caches -c tests/test-progs/spm/test
