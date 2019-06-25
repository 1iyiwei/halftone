/*
  HilbertCounter.hpp

  Li-Yi Wei
  05/01/2009

*/

#ifndef _HILBERT_COUNTER_HPP
#define _HILBERT_COUNTER_HPP

#include "Counter.hpp"

class HilbertCounter : public Counter
{
public:
    HilbertCounter(const int dimension, const int digit_min, const int digit_max);
    HilbertCounter(const int dimension, const vector<int> & digit_min, const vector<int> & digit_max);
    ~HilbertCounter(void);

    int Reset(void);

    int Get(vector<int> & value) const;
    
    int Next(void);

protected:
    static int NumBits(const vector<int> & digit_min, const vector<int> & digit_max);

protected:
    const vector<int> _digit_min;
    const vector<int> _digit_max;
    const int _num_bits; // per coordinate dimension
    vector<int> _value;
    int _count; // number of remaining valid values

    typedef unsigned long long BitMask;
    BitMask *_coord;

private:
    // no copy nor assignment due to _coord array
    HilbertCounter(const HilbertCounter & input);
    HilbertCounter & operator=(const HilbertCounter & input);
};

#endif
