/*
  RandomScanner.cpp

  Li-Yi Wei
  04/24/2009

*/

#include "RandomScanner.hpp"

RandomScanner::RandomScanner(const vector<int> & digit_min, const vector<int> & digit_max) : _counter(digit_min.size(), digit_min, digit_max)
{
    // nothing to do
}

RandomScanner::~RandomScanner(void)
{
    // nothing to do
}

int RandomScanner::Reset(void)
{
    return _counter.Reset();
}

int RandomScanner::GetCoordinate(Coordinate & coordinate) const
{
    if(! _counter.Get(_value)) return 0;

    if(coordinate.Dimension() != _value.size())
    {
        coordinate = Coordinate(_value.size());
    }

    for(int i = 0; i < coordinate.Dimension(); i++)
    {
        coordinate[i] = _value[i];
    }

    return 1;
}

int RandomScanner::GetOffset(const Coordinate & input, Coordinate & output) const
{
    // this is actually not correct.... to fix later.
    output = input;
    return 1;
}

int RandomScanner::Next(void)
{
    return _counter.Next();
}
