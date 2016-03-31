# -I option searches for specified directory

UTIL=../../../../util
INST=${UTIL}/m5/m5op_x86.S
FLAGS=-g -O2
NAME=trisolv

all: spm org

spm: $(NAME)_spm.c
	gcc -I $(UTIL) $(INST) $(NAME)_spm.c -o spm$(NN) $(FLAGS) -DUNIX --static -DN=$(NN)

org: $(NAME)_mod.c
	gcc -I $(UTIL) $(INST) $(NAME)_mod.c -o org$(NN) $(FLAGS) -DUNIX --static -DN=$(NN)

cache: $(NAME)_cachegrind.c
	gcc -g $(NAME)_cachegrind.c -o cache$(NN) -DN=$(NN)

clean:
	rm org spm
cleanc:
	rm cache*
