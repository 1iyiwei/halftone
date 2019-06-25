/*
  SerpentineScanner.cpp

  Li-Yi Wei
  04/24/2009

*/

#include "SerpentineScanner.hpp"
#include "Exception.hpp"

SerpentineScanner::SerpentineScanner(const vector<int> & digit_min, const vector<int> & digit_max) : _counter(digit_min.size(), digit_min, digit_max), _digit_min(digit_min), _digit_max(digit_max)
{
    if(_counter.Dimension() != 2)
    {
        throw Exception("SerpentineScanner supports only 2D now!");
    }
}

SerpentineScanner::~SerpentineScanner(void)
{
    // nothing to do
}

int SerpentineScanner::GetCoordinate(Coordinate & coordinate) const
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

    // flip, assume 2D
    if(_value[coordinate.Dimension()-1]%2)
    {
        coordinate[0] = _digit_min[0] + _digit_max[0] - coordinate[0];
    }

    return 1;
}

int SerpentineScanner::GetOffset(const Coordinate & input, Coordinate & output) const
{
    output = input; 

    // flip, assume 2D
    if(! _counter.Get(_value)) return 0;
    if(_value[output.Dimension()-1]%2)
    {
        output[0] *= -1;
    }

    return 1;
}

Counter & SerpentineScanner::GetCounter(void)
{
    return _counter;
}

const Counter & SerpentineScanner::GetCounter(void) const
{
    return _counter;
}

