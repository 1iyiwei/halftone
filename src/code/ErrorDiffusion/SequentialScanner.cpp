/*
  SequentialScanner.cpp

  Li-Yi Wei
  04/24/2009

*/

#include "SequentialScanner.hpp"

SequentialScanner::SequentialScanner(const vector<int> & digit_min, const vector<int> & digit_max) : _counter(digit_min.size(), digit_min, digit_max)
{
    // nothing to do
}

SequentialScanner::~SequentialScanner(void)
{
    // nothing to do
}

Counter & SequentialScanner::GetCounter(void)
{
    return _counter;
}

const Counter & SequentialScanner::GetCounter(void) const
{
    return _counter;
}
