#!/bin/sh

################################################
# trmm

dir=./tests/test-progs/spm/trmm

echo $#

if [ $# = 1 ]
then
   # 1 A in main memory, B in SPM
   ./build/X86/gem5.debug --stats-file=trmm-spm-1.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2ns --spm-w-lat-1=3ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

   # 2 A in main memory, B in SPM
   ./build/X86/gem5.debug --stats-file=trmm-spm-2.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2.5ns --spm-w-lat-1=3.2ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

   # 3 A in main memory, B in SPM
   ./build/X86/gem5.debug --stats-file=trmm-spm-3.txt configs/spm/se.py --mem-size=4GB --scratchpad=1 --spm-size-1=16MB --spm-type-1=1 --spm-r-lat-1=2.2ns --spm-w-lat-1=2.5ns --caches --cpu-type=timing -c tests/test-progs/spm/trmm/spm

   # 4 Both in main memory: see configs/common/Options.py to check the cache configuration
   ./build/X86/gem5.debug --stats-file=trmm-no-spm.txt configs/example/se.py --mem-size=4GB --caches --cpu-type=timing -c tests/test-progs/spm/trmm/org

   # 5 cache l2
   ./build/X86/gem5.debug --stats-file=trmm-no-spm.txt configs/example/se.py --mem-size=4GB --caches --cpu-type=timing -c tests/test-progs/spm/trmm/org
fi

## Cachegrind
if [ $# > 3 ]
then
    valgrind --tool=cachegrind --cachegrind-out-file=trmm-cachegrind-$1.out $dir/cache$1
    cg_annotate --auto=yes trmm-cachegrind-$1.out
else
    echo "Wrong number of parameters: 4 needed at least\n"
fi
