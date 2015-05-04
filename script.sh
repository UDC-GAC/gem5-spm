#!/bin/bash

# No SPM 
#./build/X86/gem5.opt configs/example/se.py --caches -c tests/spm/test 

# With SPM
./build/X86/gem5.opt configs/spm/se.py --mem-size=100kB --scratchpad --spm-size=32MB --caches -c tests/test-progs/spm/test
