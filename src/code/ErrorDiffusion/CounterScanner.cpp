/*
  CounterScanner.cpp

  Li-Yi Wei
  04/24/2009

*/

#include "CounterScanner.hpp"
#include "Exception.hpp"

CounterScanner::CounterScanner(void) : _dummy(1, 0, 0)
{
    // nothing to do
}

CounterScanner::~CounterScanner(void)
{
    // nothing to do
}

int CounterScanner::Reset(void)
{
    Counter & counter = GetCounter();
    return counter.Reset();
}

int CounterScanner::GetCoordinate(Coordinate & coordinate) const
{
    const Counter & counter = GetCounter();

    if(! counter.Get(_value)) return 0;

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

int CounterScanner::GetOffset(const Coordinate & input, Coordinate & output) const
{
    output = input;
    return 1;
}

int CounterScanner::Next(void)
{
    Counter & counter = GetCounter();

    return counter.Next();
}

Counter & CounterScanner::GetCounter(void)
{
    throw Exception("CounterScanner::GetCounter() should not be called");
    return _dummy;
}

const Counter & CounterScanner::GetCounter(void) const
{
    throw Exception("CounterScanner::GetCounter() should not be called");
    return _dummy;
}
