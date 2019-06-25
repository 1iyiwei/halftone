/*
  Scanner.cpp

  Li-Yi Wei
  04/24/2009

*/

#include "Scanner.hpp"
#include "Exception.hpp"

Scanner::~Scanner(void)
{
    // nothing to do
}

int Scanner::Reset(void)
{
    throw Exception("Scanner::Reset() should not be called");
    return 0;
}

int Scanner::GetCoordinate(Coordinate & coordinate) const
{
    throw Exception("Scanner::GetCoordinate() should not be called");
    return 0;
}

int Scanner::GetOffset(const Coordinate & input, Coordinate & output) const
{
    output = input;
    return 1;
}

int Scanner::Next(void)
{
    throw Exception(" Scanner::Next() should not be called");
    return 0;
}
