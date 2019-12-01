Example of how it is called from R layer

SEXP fft(SEXP z, SEXP inverse)
{
    SEXP d;
    int i, inv, maxf, maxmaxf, maxmaxp, maxp, n, ndims, nseg, nspn;
    double *work;
    int *iwork;
    size_t smaxf;
    size_t maxsize = ((size_t) -1) / 4;

    switch (TYPEOF(z)) {
    case INTSXP:
    case LGLSXP:
    case REALSXP:
    z = coerceVector(z, CPLXSXP);
    break;
    case CPLXSXP:
    if (MAYBE_REFERENCED(z)) z = duplicate(z);
    break;
    default:
    error(_("non-numeric argument"));
    }
    PROTECT(z);

    /* -2 for forward transform, complex values */
    /* +2 for backward transform, complex values */

    inv = asLogical(inverse);
    if (inv == NA_INTEGER || inv == 0)
    inv = -2;
    else
    inv = 2;

    if (LENGTH(z) > 1) {
    if (isNull(d = getAttrib(z, R_DimSymbol))) {  /* temporal transform */
        n = length(z);
        fft_factor(n, &maxf, &maxp);
        if (maxf == 0)
        error(_("fft factorization error"));
        smaxf = maxf;
        if (smaxf > maxsize)
        error("fft too large");
        work = (double*)R_alloc(4 * smaxf, sizeof(double));
        iwork = (int*)R_alloc(maxp, sizeof(int));
        fft_work(&(COMPLEX(z)[0].r), &(COMPLEX(z)[0].i),
             1, n, 1, inv, work, iwork);
