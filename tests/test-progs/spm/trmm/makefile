# -I option searches for specified directory

UTIL=/home/markoshorro/gem5/util
INST=/home/markoshorro/gem5/util/m5/m5op_x86.S

all: spm org

spm:
	gcc -I $(UTIL) $(INST) trmm_spm.c -o spm -DUNIX --static

org:
	gcc -I $(UTIL) $(INST) trmm_mod.c -o org -DUNIX --static

cache:
	gcc -g trmm_cachegrind.c -o cache

cache2:
	gcc -g -O2 trmm_cachegrind.c -o cache2

cache3:
	gcc -g -O3 trmm_cachegrind.c -o cache3

clean:
	rm org spm cache
