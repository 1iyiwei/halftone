/*
  HilbertScanner.cpp

  Li-Yi Wei
  05/01/2009

*/

#include "HilbertScanner.hpp"

HilbertScanner::HilbertScanner(const vector<int> & digit_min, const vector<int> & digit_max) : _counter(digit_min.size(), digit_min, digit_max)
{
    // nothing to do
}

HilbertScanner::~HilbertScanner(void)
{
    // nothing to do
}

int HilbertScanner::GetOffset(const Coordinate & input, Coordinate & output) const
{
    // in theory this is not correct
    // as I should orient with respect to the current orientation
    // but in practice a symmetric neighborhood is likely to be used
    return CounterScanner::GetOffset(input, output);
}

Counter & HilbertScanner::GetCounter(void)
{
    return _counter;
}

const Counter & HilbertScanner::GetCounter(void) const
{
    return _counter;
}
