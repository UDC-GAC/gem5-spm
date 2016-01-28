/*
 * Universidade da Coruña. 2015
 *
 * Marcos Horro Varela
 *
 * Testing pseudo_instructions
 */

#define KBYTE 1024
#define MBYTE 1024*1024
#define DEBUG 0

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv)
{
  printf("test program start\n");

  int *p = (int *) malloc(sizeof(int)*8*KBYTE);

  int i = 0;

  if (DEBUG)  {
    printf("Virtual address pointer vaddr: %lu\n", p);
    exit(0);
  }
  
  for (i=0; i<KBYTE; i++) {
    p[i] = i; 
    printf("dir: %d\n", p[i]);
  }

  printf("test program finish\n");
  
}
