# -I option searches for specified directory

UTIL=../../../../util
INST=${UTIL}/m5/m5op_x86.S

all: spm org

spm: 2mm_spm.c
	gcc -I $(UTIL) $(INST) 2mm_spm.c -o spm -DUNIX --static

org: 2mm_mod.c
	gcc -I $(UTIL) $(INST) 2mm_mod.c -o org -DUNIX --static

clean:
	rm org spm
