# -I option searches for specified directory

UTIL=/home/markoshorro/gem5/util
INST=/home/markoshorro/gem5/util/m5/m5op_x86.S

all:
	gcc -I utilities utilities/polybench.c -I $(UTIL) $(INST) 2mm.c -o 2mm -DUNIX --static

clean:
	rm 2mm
