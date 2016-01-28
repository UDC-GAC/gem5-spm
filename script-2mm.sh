#!/bin/bash

################################################
# @TODO
# 2MM
#./build/X86/gem5.debug --stats-file=spm2mm.txt --debug-flags=PseudoInst --debug-file=trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=32MB --caches -c tests/test-progs/spm/2mm/2mm

# 2mm ORG
#./build/X86/gem5.debug --stats-file=2mm-no-spm.txt --debug-flags=PseudoInst --debug-file=2mm-trace.out configs/example/se.py --mem-size=4GB --caches -c tests/test-progs/spm/2mm/org

# 2mm SPM no cache
#./build/X86/gem5.debug --stats-file=2mm-spm.txt --debug-flags=PseudoInst --debug-file=2mm-spm-trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=128MB --spm-type-1=2 -c tests/test-progs/spm/2mm/spm

# 2mm SPM cache
#./build/X86/gem5.debug --stats-file=2mm-spm.txt --debug-flags=PseudoInst --debug-file=2mm-spm-trace.out configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=128MB --spm-type-1=2 --caches -c tests/test-progs/spm/2mm/spm

# Both in SPM

