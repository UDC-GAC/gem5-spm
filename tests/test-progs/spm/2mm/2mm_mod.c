/**
 * 2mm.c: version by Markos Horro
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "m5/m5op.h"

/* Include benchmark-specific header. */
/* Default data type is double, default size is 4000. */
#include "2mm.h"

#ifndef SPM_VAR
# define KBYTE      1024
# define MBYTE      1024*1024
# define SPM_1      1
# define SPM_2      2
# define SPM_3      3
# define SPM_SIZE_1 MBYTE*128
# define SPM_SIZE_2 MBYTE*128
# define SPM_SIZE_3 MBYTE*128
#endif

int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  int nl = NL;

  /* Variable declaration/allocation. */
  double alpha;
  double beta; 

  double *vaddr1 = (double *) malloc(256*MBYTE*sizeof(double));

  double *tmp = vaddr1;
  double *A = vaddr1 + KBYTE*sizeof(double);
  double *B = A + KBYTE*sizeof(double);
  double *C = B + KBYTE*sizeof(double);
  double *D = C + KBYTE*sizeof(double);

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

#pragma scop
  /* D := alpha*A*B*C + beta*D */
  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
      {
	tmp[i*NJ+j] = 0;
	for (k = 0; k < NK; ++k)
	  tmp[i*NJ+j] += alpha * A[i*NK+k] * B[k*NJ+j];
      }
  for (i = 0; i < NI; i++)
    for (j = 0; j < NL; j++)
      {
	D[i*NL+j] *= beta;
	for (k = 0; k < NJ; ++k)
	  D[i*NL+j] += tmp[i*NK+k] * C[k*NJ+j];
      }
#pragma endscop

  return 0;
}
