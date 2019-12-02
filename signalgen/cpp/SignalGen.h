#ifndef __SIGNALGEN_H
#define __SIGNALGEN_H

#include <stdio.h>
#include <vector>

class SignalGen
{
private:
    int _sps;
    const int maxHarmonics = 1000;
    const int table_size = 1024;
    float* _table = new float[table_size];
    float* _out;
    int _npoints;
    bool GenerateOne(int freq, float amp, float& phase);

    void init (float duration_sec);


public:
    SignalGen(int sps);
    bool Generate(int freq, const std::vector<float>& harmAmps, const std::vector<float>& phases, float duration_sec);
    bool GenerateOne(int freq, float amp, float& phase, float duration_sec);
    void Get(int& count, float*& out) const { count = _npoints; out = _out; }
    ~SignalGen();
};


#endif