#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "m5/m5op.h"

#ifndef NX
#define NX          1000
#endif

int main(int argc, char** argv)
{
    /* Retrieve problem size. */
    int i,j,k;
    int n = NX;
    /* Variable declaration/allocation. */
    double *y = (double *) malloc(NX*NX*sizeof(double));
    double *sum = (double *) malloc(NX*NX*sizeof(double));
    double *alpha = (double *) malloc(NX*sizeof(double));
    double *beta = (double *) malloc(NX*sizeof(double));
    double *r = (double *) malloc(NX*sizeof(double));
    double *out = (double *) malloc(NX*sizeof(double));
  
    for (i = 0; i < n; i++)
	{
	    alpha[i] = i;
	    beta[i] = (i+1)/n/2.0;
	    r[i] = (i+1)/n/4.0;
	    for (j = 0; j < n; j++) {
		y[i*n + j] = ((double) i*j) / n;
		sum[i*n + j] = ((double) i*j) / n;
	    }
	}

#pragma scop
    y[0] = r[0];
    beta[0] = 1;
    alpha[0] = r[0];
    for (k = 1; k < NX; k++)
	{
	    beta[k] = beta[k-1] - alpha[k-1] * alpha[k-1] * beta[k-1];
	    sum[k] = r[k];
	    for (i = 0; i <= k - 1; i++)
		sum[(i+1)*n + k] = sum[i*n + k] + r[k-i-1] * y[i*n + k-1];
	    alpha[k] = -sum[k*n + k] * beta[k];
	    for (i = 0; i <= k-1; i++)
		y[i*n + k] = y[i*n + k-1] + alpha[k] * y[(k-i-1)*n + k-1];
	    y[k*n + k] = alpha[k];
	}
    for (i = 0; i < NX; i++)
	out[i] = y[i*n + NX-1];
#pragma endscop

    /* Prevent dead-code elimination. All live-out data must be printed
       by the function call in argument. */

    /* Be clean. */
    printf("%f %f %f %f %f %f\n", y[11], sum[11], out[11], alpha[11], r[11], beta[11]);

    return 0;
}
