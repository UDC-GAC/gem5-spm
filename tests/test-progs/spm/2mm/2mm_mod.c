/**
 * 2mm.c: version by Markos Horro
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "m5/m5op.h"

#ifndef SPM_VAR
# define KBYTE      1024
# define MBYTE      1024*1024
# define SPM_1      1
# define SPM_2      2
# define SPM_3      3
# define SPM_SIZE_1 MBYTE*128
# define SPM_SIZE_2 MBYTE*128
# define SPM_SIZE_3 MBYTE*128
# define NI         32
# define N          32*32
#endif

int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NI;
  int nk = NI;
  int nl = NI;

  /* Variable declaration/allocation. */
  double alpha;
  double beta; 

  double *vaddr1 = (double *) malloc(256*MBYTE*sizeof(double));

  double *tmp = vaddr1;
  double *A = vaddr1 + N*sizeof(double);
  double *B = A + N*sizeof(double);
  double *C = B + N*sizeof(double);
  double *D = C + N*sizeof(double);

  /* Run kernel. */
  int i, j, k;

  alpha = 32412;
  beta = 2123;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i*nk+j] = ((double) i*j) / ni;
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i*nj+j] = ((double) i*(j+1)) / nj;
  for (i = 0; i < nl; i++)
    for (j = 0; j < nj; j++)
      C[i*nj+j] = ((double) i*(j+3)) / nl;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++)
      D[i*nl+j] = ((double) i*(j+2)) / nk;

  m5_reset_stats(0,0);
#pragma scop
  /* D := alpha*A*B*C + beta*D */
  for (i = 0; i < NI; i++)
    for (j = 0; j < NI; j++)
      {
	tmp[i*NI+j] = 0;
	for (k = 0; k < NI; ++k)
	  tmp[i*NI+j] += alpha * A[i*NI+k] * B[k*NI+j];
      }
  for (i = 0; i < NI; i++)
    for (j = 0; j < NI; j++)
      {
	D[i*NI+j] *= beta;
	for (k = 0; k < NI; ++k)
	  D[i*NI+j] += tmp[i*NI+k] * C[k*NI+j];
      }
#pragma endscop
  m5_dump_stats(0,0);

  return 0;
}
