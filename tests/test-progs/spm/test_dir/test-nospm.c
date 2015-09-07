/*
 * Universidade da Coruña. 2015
 *
 * Marcos Horro Varela
 *
 * Testing pseudo_instructions
 */

#define KBYTE 1024
#define MBYTE 1024*1024
#define DEBUG 1

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char** argv)
{
  printf("test program start\n");

  int *p = (int *) malloc(sizeof(int)*KBYTE*64);

  int i = 0;

  if (DEBUG)
    printf("Virtual address pointer vaddr: %lu\n", p);

  for (i=0; i<KBYTE*64; i++) {
    p[i] = i;
    if (DEBUG) 
      printf("dir: %lu\n", &p[i]);
  }

  printf("test program finish\n");
  
}
