# -I option searches for specified directory

UTIL=/home/markoshorro/gem5/util
INST=/home/markoshorro/gem5/util/m5/m5op_x86.S

all:
	gcc -I utilities utilities/polybench.c -I $(UTIL) $(INST) durbin.c -o durbin -DUNIX --static

test:
	gcc test.c -o test -I $(UTIL) $(INST) -DUNIX --static

testn:
	gcc test-nospm.c -o testn -DUNIX --static

clean:
	rm test testn durbin
