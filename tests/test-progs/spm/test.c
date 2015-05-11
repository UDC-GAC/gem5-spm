/*
 * Testing pseudo_instructions
 */

#define KBYTE 1024

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

  int *p = (int *) spm_malloc(0, sizeof(int)*KBYTE);

  int i = 0;

  if (DEBUG)
    printf("El puntero vaddr, tiene la dirección: %lu\n", p);

  for (i=0; i<KBYTE; i++) {
    p[i] = i;
    if (DEBUG) 
      printf("dir: %lu\n", &p[i]);
  }

  printf("test program finish\n");
  
}
