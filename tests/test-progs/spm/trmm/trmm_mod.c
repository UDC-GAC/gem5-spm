/**
 * trmm.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include benchmark-specific header. */
/* Default data type is double, default size is 4000. */
#include "trmm.h"

int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = 1024;

  /* Variable declaration/allocation. */
  double alpha;

  double *vaddr = (double *) malloc(4*ni*sizeof(double));

  double *A = vaddr;
  double *B = A + 2 * ni * sizeof(double);

  printf("Llego: %d, %lu, %lu\n", 4*ni*sizeof(double), A, B);
  
  /* Run kernel. */
  int i, j, k;
  alpha = 32412;
  for (i = 0; i < ni; i++)
    for (j = 0; j < ni; j++) {
      A[i*ni + j] = ((double) i*j) / ni;
      B[i*ni + j] = ((double) i*j) / ni;
      printf("(%d,%d): %lu, %lu\n", i, j, &A[i*ni + j], &B[i*ni + j]);
    }
#pragma scop
  /*  B := alpha*A'*B, A triangular */
  for (i = 1; i <  ni; i++)
    for (j = 0; j < ni; j++)
      for (k = 0; k < i; k++)
        B[i*ni + j] += alpha * A[i*ni + k] * B[j*ni + k];
#pragma endscop

  return 0;
}
