#ifndef __FFT_H
#define __FFT_H

#define TRUE 1 
#define FALSE 0

typedef int BOOL;
#define imax2(x, y) ((x < y) ? y : x)
#define imin2(x, y) ((x < y) ? x : y);

#ifndef M_SQRT_3
#define M_SQRT_3	1.732050807568877293527446341506	/* sqrt(3) */
#endif

#include <stdio.h>

BOOL fft_factor(int n, int *pmaxf, int *pmaxp);

//Re(a) + Im(b) input and Re + Im output
BOOL fft_work(double *a, double *b, int nseg, int n, int nspn, int isn, double *work, int *iwork);

BOOL fft (double *re, int len, double* amps, double* phases);


#endif
