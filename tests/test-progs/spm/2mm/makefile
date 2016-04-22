# -I option searches for specified directory

UTIL=../../../../util
INST=${UTIL}/m5/m5op_x86.S
FLAGS=-g -O2
NAME=2mm

all: spm org

spm: $(NAME)_spm.c
	gcc -I $(UTIL) $(INST) $(NAME)_spm.c -o spm$(NN) $(FLAGS) -DUNIX --static -DNX=$(NN)

org: $(NAME)_mod.c
	gcc -I $(UTIL) $(INST) $(NAME)_mod.c -o org$(NN) $(FLAGS) -DUNIX --static -DNX=$(NN)

clean:
	rm org spm
