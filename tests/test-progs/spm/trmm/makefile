# -I option searches for specified directory

UTIL=/home/markoshorro/gem5/util
INST=/home/markoshorro/gem5/util/m5/m5op_x86.S

all: spm org

spm:
	gcc -I $(UTIL) $(INST) trmm_spm.c -o spm -DUNIX --static

org:
	gcc trmm_mod.c -o org -DUNIX --static

clean:
	rm org spm
