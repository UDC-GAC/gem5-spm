# -I option searches for specified directory

UTIL=../../../../util
INST=${UTIL}/m5/m5op_x86.S
FLAGS=-O2 -o
all: spm org

spm: trmm_spm.c
	gcc -I $(UTIL) $(INST) trmm_spm.c $(FLAGS) spm -DUNIX --static

org: trmm_mod.c
	gcc -I $(UTIL) $(INST) trmm_mod.c $(FLAGS) org -DUNIX --static

alpha: trmm_spm_alpha.c
	gcc -I $(UTIL) $(INST) trmm_spm_alpha.c -o alpha -DUNIX --static

cache1: trmm_cachegrind.c
	gcc -g trmm_cachegrind.c -o cache1

cache2: trmm_cachegrind.c
	gcc -g -O2 trmm_cachegrind.c -o cache2

cache3: trmm_cachegrind.c
	gcc -g -O3 trmm_cachegrind.c -o cache3

clean:
	rm org spm alpha
cleanc:
	rm cache1 cache2 cache3
