#ifndef __SIGNALGEN_H
#define __SIGNALGEN_H

#include <stdio.h>
#include <vector>

class SignalGen
{
private:
    const int maxHarmonics = 100;
    const int table_size = 1024;
    float* _table = new float[table_size];
    float* _out;
    int _npoints;

    bool GenerateFreq(int freq, float amp, int sps);

public:
    SignalGen();
    bool Generate(int freq, const std::vector<float>& harmAmps, int sps, float duration_sec);
    void Get(int& count, float*& out) const { count = _npoints; out = _out; }
    ~SignalGen();
};


#endif