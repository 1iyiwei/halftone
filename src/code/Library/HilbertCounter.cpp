/*
  HilbertCounter.cpp

  Li-Yi Wei
  05/01/2009
  
*/

#include <iostream> // for debug
using namespace std;

#include <math.h>

#include "HilbertCounter.hpp"
#include "hilbert/hilbert.h"

#include "Exception.hpp"

HilbertCounter::HilbertCounter(const int dimension, const int digit_min, const int digit_max) : Counter(dimension), _digit_min(vector<int>(dimension, digit_min)), _digit_max(vector<int>(dimension, digit_max)), _num_bits(NumBits(_digit_min, _digit_max)), _value(dimension), _count(0), _coord(new BitMask[dimension])
{
    if(_num_bits <= 0) throw Exception("HilbertCounter::HilbertCounter(): illegal inputs");

    Reset();
}

HilbertCounter::HilbertCounter(const int dimension, const vector<int> & digit_min, const vector<int> & digit_max) : Counter(dimension), _digit_min(digit_min), _digit_max(digit_max), _num_bits(NumBits(_digit_min, _digit_max)), _value(dimension), _count(0), _coord(new BitMask[dimension])
{
    if(_num_bits <= 0) throw Exception("HilbertCounter::HilbertCounter(): illegal inputs");

    Reset();
}

HilbertCounter::~HilbertCounter(void)
{
    if(_coord)
    {
        delete _coord;
    }
}

int HilbertCounter::Reset(void)
{
    _value = _digit_min;
    _count = pow(pow(2.0, _num_bits), static_cast<int>(_value.size()))-1;

    return 1;
}

int HilbertCounter::Get(vector<int> & value) const
{
    value = _value;

    return 1;
}

int HilbertCounter::Next(void)
{
    const bool over = (_count <= 0);

    if(!over)
    {
        for(unsigned int i = 0; i < _value.size(); i++)
        {
            _coord[i] = _value[i] - _digit_min[i];
        }

        hilbert_incr(_value.size(), _num_bits, _coord);

        for(unsigned int i = 0; i < _value.size(); i++)
        {
            _value[i] = _coord[i] + _digit_min[i];
        }

        _count--;
    }

    // done
    return !over;
}

int HilbertCounter::NumBits(const vector<int> & digit_min, const vector<int> & digit_max)
{
    if((digit_min.size() <= 0) || (digit_min.size() != digit_max.size()))
    {
        return 0;
    }

    int range = digit_max[0]-digit_min[0]+1;

    if(range <= 0)
    {
        return 0;
    }

    for(unsigned int i = 0; i < digit_min.size(); i++)
    {
        if((digit_max[i]-digit_min[i]+1) != range) return 0;
    }

    int num_bits = 0;
    
    while(range > 1)
    {
        if(range%2) return 0;

        range /= 2; num_bits++;
    }

    return num_bits;
}

HilbertCounter::HilbertCounter(const HilbertCounter & input) : Counter(input._digit_min.size()), _digit_min(input._digit_min), _digit_max(input._digit_max), _num_bits(NumBits(_digit_min, _digit_max)), _value(input._value), _coord(new BitMask[input._digit_min.size()])
{
    throw Exception("HilbertCounter::HilbertCounter() copy constructor should not be called");
}

HilbertCounter & HilbertCounter::operator=(const HilbertCounter & input)
{
    throw Exception(" HilbertCounter::operator=() assignment operator should not be called");
}
