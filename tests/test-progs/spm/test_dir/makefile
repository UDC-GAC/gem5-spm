# -I option searches for specified directory

UTIL=../../../../util
INST=${UTIL}/m5/m5op_x86.S

all: test testn 

test: test.c
	gcc test.c -o test -I $(UTIL) $(INST) -DUNIX --static

testn: test-nospm.c
	gcc test-nospm.c -o testn -I $(UTIL) $(INST) -DUNIX --static

clean:
	rm test testn
