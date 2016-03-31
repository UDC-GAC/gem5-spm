/**
 * bicg.c: This file is part of the PolyBench/C 3.2 test suite.
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

#ifndef NX
#define NX          1000
#endif

#ifndef M_PI
#define M_PI 3.141592
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
  int i, j;
  int nx = NX;
  int ny = NX;

  /* Variable declaration/allocation. */
  double *A = (double *) malloc(ny*nx*sizeof(double));
  double *s = (double *) malloc(ny*sizeof(double));
  double *q = (double *) malloc(nx*sizeof(double));
  double *r = (double *) malloc(ny*sizeof(double));
  double *p = (double *) malloc(nx*sizeof(double));

  for (i = 0; i < ny; i++)
    p[i] = i * M_PI;
  for (i = 0; i < nx; i++) {
    r[i] = i * M_PI;
    for (j = 0; j < ny; j++)
      A[i*nx + j] = ((double) i*(j+1))/nx;
  }

  m5_reset_stats(0,0);
#pragma scop
  for (i = 0; i < nx; i++)
    s[i] = 0;
  for (i = 0; i < nx; i++)
    {
      q[i] = 0;
      for (j = 0; j < ny; j++)
	{
	  s[j] = s[j] + r[i] * A[i*nx + j];
	  q[i] = q[i] + A[i*nx + j] * p[j];
	}
    }
#pragma endscop
  m5_dump_stats(0,0);

  printf("%f, %f, %f, %f, %f\n", A[11], p[11], q[11], r[11], s[11]);

  return 0;
}
