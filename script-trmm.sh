#!/bin/sh

################################################
# trmm

# Local variables
usage="Parameters needed:\n\t* exec: executing trmm benchmarks\n\t* cache <number>: executes cachegrind tests for <number> configuration"
error="Wrong number of parameters: 1 needed at least"
dir=./tests/test-progs/spm/trmm

# Script
if [ $# = 0 ]
then
    echo $error
    echo $usage
    return
fi

if [ $1 = "exec" ]
then
   # 1 A in main memory, B in SPM (2/3)
   ./build/X86/gem5.debug --stats-file=trmm-spm-1.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=3ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

   # 2 A in main memory, B in SPM (2.5/3.2)
   ./build/X86/gem5.debug --stats-file=trmm-spm-2.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2.5ns --spm-w-lat-1=3.2ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

   # 3 A in main memory, B in SPM (2.2/2.5)
   ./build/X86/gem5.debug --stats-file=trmm-spm-3.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2.2ns --spm-w-lat-1=2.5ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

   # 4 A in main memory, B in SPM (2/2)
   ./build/X86/gem5.debug --stats-file=trmm-spm-4.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=2ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

   # 5 Both in main memory: see configs/common/Options.py to check the cache configuration
   ./build/X86/gem5.debug --stats-file=trmm-nospm-convarch.txt configs/example/se.py --mem-size=4GB --caches --cpu-type=timing -c tests/test-progs/spm/trmm/org

   # 6 Using another architecture
   ./build/X86/gem5.debug --stats-file=trmm-nospm-spmarch.txt configs/spm/se.py --mem-size=4GB --caches --cpu-type=timing -c tests/test-progs/spm/trmm/org
elif [ $1 = "cache" ]
then
    valgrind --tool=cachegrind --cachegrind-out-file=trmm-cachegrind-$2.out $dir/cache$2
    cg_annotate --auto=yes trmm-cachegrind-$2.out
fi
