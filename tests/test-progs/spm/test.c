/*
 * Testing pseudo_instructions
 */

#define KBYTE 1024

#include <stdio.h>
#include <stdlib.h>
#include "m5/m5op.h"

int main(int argc, char** argv)
{
  printf("test\n");

  long unsigned *p = (long unsigned *) malloc(KBYTE * KBYTE * 16);

  *p = p;

  printf("addr: %lu (%x). content: %lu\n", p, p, *p);
  
  spm_malloc(*p, KBYTE*KBYTE*16);

  free(p);
  
}
