/**
 * trisolv.c: This file is part of the PolyBench/C 3.2 test suite.
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

#ifndef N
#define N 1000
#endif

int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int i, j;
  int n = N;

  /* Variable declaration/allocation. */
  double *A = (double *) malloc(n*n*sizeof(double));
  double *x = (double *) malloc(n*n*sizeof(double));
  double *c = (double *) malloc(n*n*sizeof(double));

  for (i = 0; i < n; i++)
    {
      c[i] = x[i] = ((double) i) / n;
      for (j = 0; j < n; j++)
	A[i*n + j] = ((double) i*j) / n;
    }

  /* Run kernel. */
  m5_reset_stats(0,0);
#pragma scop
  for (i = 0; i < N; i++)
    {
      x[i] = c[i];
      for (j = 0; j <= i - 1; j++)
        x[i] = x[i] - A[i*n + j] * x[j];
      x[i] = x[i] / A[i*n + i];
    }
#pragma endscop
  m5_dump_stats(0,0);

  printf("%f, %f, %f\n", A[11], c[11], x[11]);

  return 0;
}
