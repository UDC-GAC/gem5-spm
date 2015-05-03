/*
 * Testing pseudo_instructions
 */

#include <stdio.h>
#include <stdlib.h>
#include "m5/m5op.h"

int main(int argc, char** argv)
{
  printf("test\n");

  int *p = (int *) malloc(1000);

  *p = 1;

  printf("%d\n", *p);

  free(p);
  
  //spm_malloc(4096);
}
