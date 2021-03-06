/**
 * gemm.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "m5/m5op.h"

#ifndef NI
#define NI 200
#endif

#ifndef NJ
#define NJ 200
#endif

#ifndef NK
#define NK 200
#endif

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
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  int i, j, k;

  /* Variable declaration/allocation. */
  double *alpha;
  double *beta;
  double *A = (double *) malloc(ni*nk*sizeof(double));
  double *B = (double *) malloc(nk*nj*sizeof(double));
  double *C = (double *) spm_malloc(SPM_SIZE_1, SPM_1);

  /* Initialize array(s). */
  *alpha = 32412;
  *beta = 2123;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++)
      C[i*ni + j] = ((double) i*j) / ni;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i*ni + j] = ((double) i*j) / ni;
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i*nk + j] = ((double) i*j) / ni;

  /* Run kernel. */
  m5_reset_stats(0,0);
#pragma scop
  /* C := alpha*A*B + beta*C */
  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
      {
	C[i*NI + j] *= *beta;
	for (k = 0; k < NK; ++k)
	  C[i*NI + j] += *alpha * A[i*NI + k] * B[k*NK + j];
      }
#pragma endscop
  m5_dump_stats(0,0);

  printf("%f, %f, %f\n", C[11], A[11], B[11]);

  return 0;
}
