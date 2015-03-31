# -I option searches for specified directory

all:
	gcc -I utilities utilities/polybench.c -I /home/markoshorro/gem5/util /home/markoshorro/gem5/util/m5/m5op_x86.S durbin.c -o durbin -DUNIX --static

test:
	gcc test.c -o test -I /home/markoshorro/gem5/util /home/markoshorro/gem5/util/m5/m5op_x86.S --static
