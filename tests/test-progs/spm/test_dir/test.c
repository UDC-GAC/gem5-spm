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
#include <string.h>
#include <math.h>
#include "m5/m5op.h"

int main(int argc, char** argv)
{
  printf("test program start\n");
  
  int *p = (int *) spm_malloc(sizeof(int)*MBYTE*2, 1);

  int i = 0;

  if (DEBUG)
    printf("Virtual address pointer vaddr: %lu\n", p);

  //m5_checkpoint(0,0);
  for (i=0; i<KBYTE; i++) {
    p[i] = i;
    if (DEBUG) 
      printf("dir: %lu\n", &p[i]);
  }
  //m5_exit(0);

  printf("test program finish\n");
  
}
