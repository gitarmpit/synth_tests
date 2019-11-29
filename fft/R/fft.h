#ifndef __FFT_H
#define __FFT_H

#define TRUE 1 
#define FALSE 0

typedef int BOOL;

#include <stdio.h>

BOOL fft_factor(int n, int *pmaxf, int *pmaxp);

//Re(a) + Im(b) input and Re + Im output
BOOL fft_work(double *a, double *b, int nseg, int n, int nspn, int isn, double *work, int *iwork);

#endif
