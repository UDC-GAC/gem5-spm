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
#include "m5/m5op.h"

/* Include benchmark-specific header. */
/* Default data type is double, default size is 4000. */
#include "trmm.h"

#ifndef SPM_VAR
# define KBYTE      1024
# define MBYTE      1024*1024
# define SPM_1      1
# define SPM_2      2
# define SPM_3      3
# define SPM_SIZE_1 MBYTE*16
# define SPM_SIZE_2 MBYTE*8
# define SPM_SIZE_3 MBYTE*8
#endif

int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = 1024;

  /* Variable declaration/allocation. */
  double alpha;

  double *vaddr = (double *) spm_malloc(SPM_SIZE_1, SPM_1);
  double *B = vaddr;
  double *A = (double *) malloc(2*ni*sizeof(double));
  
  /* Run kernel. */
  int i, j, k;
  alpha = 32412;
  for (i = 0; i < ni; i++)
    for (j = 0; j < ni; j++) {
      A[i*ni + j] = ((double) i*j) / ni;
      B[i*ni + j] = ((double) i*j) / ni;
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
