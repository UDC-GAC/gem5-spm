# -I option searches for specified directory

UTIL=/home/markoshorro/gem5/util
INST=/home/markoshorro/gem5/util/m5/m5op_x86.S

all: test testn testtmp

test:
	gcc test.c -o test -I $(UTIL) $(INST) -DUNIX --static

testn:
	gcc test-nospm.c -o testn -DUNIX --static

testtmp:
	gcc test_tmp.c -o testmp -I $(UTIL) $(INST) -DUNIX --static

clean:
	rm test testn testmp
