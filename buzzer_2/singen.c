#include "singen.h"

static double* table;

int singen(int table_size, int sps, int freq, float *phase, float duration_sec, short amp, size_t* length, double** out)
{
    if (!table)
    {
        table = (double*)malloc(table_size * sizeof(double));
    }

    for (int i = 0; i < table_size; ++i)
    {
        table[i] = sin(2. * M_PI / (double)table_size * (double)i);
    }

    float phase_step = (float)freq / (float)sps * (float)table_size;

    if ((int)phase_step > table_size / 2)
    {
        printf("error: phase_step too high: %7.3f\n", phase_step);
        return 1;
    }

    *length = (size_t)((double)sps * duration_sec);

    double* wav = (double*)malloc(*length * sizeof(double));

    double val;
    for (int i = 0; i < *length; ++i)
    {
        int idx0 = (int)*phase;

        double v0 = table[idx0];
        if (i < *length - 1)
        {
            int idx1 = idx0 + 1;
            if (idx1 > table_size - 1)
            {
                idx1 = 0;
            }
            double v1 = table[idx1];
            double frac = *phase - (double)idx0;
            val = v0 + (v1 - v0) * frac;
        }
        else
        {
            val = v0;
        }

        wav[i] = amp * val;
        *phase += phase_step;
        if (*phase >= (float)table_size)
            *phase -= (float)table_size;

        if ((int)*phase > table_size - 1)
        {
            fprintf(stderr, "error: phase > table size\n");
            return 1;
        }

    }

    *out = wav;
    return 0;
}