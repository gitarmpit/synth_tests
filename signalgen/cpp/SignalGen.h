#ifndef __SIGNALGEN_H
#define __SIGNALGEN_H

#include <stdio.h>
#include <vector>

class SignalGen
{
private:
    int _sps;
    const int maxHarmonics = 1000;
    const int table_size = 1000;
    double* _table = new double[table_size];
    double* _out;
    int _nsamples;
    bool GenerateOne(int freq, double amp, double& phase);

    bool init (double duration_sec);

    void ApplyEnvelope(double amp);


public:
    SignalGen(int sps, int shape = 0);
    bool Generate(int freq, const std::vector<double>& harmAmps, const std::vector<double>& phases, double duration_sec);
    bool GenerateOne(int freq, double amp, double& phase, double duration_sec);
    void Get(int& count, double*& out) const { count = _nsamples; out = _out; }
    ~SignalGen();
};


#endif