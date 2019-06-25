/*
  Constrainer.cpp

  Li-Yi Wei
  03/02/2010

*/

#include "Constrainer.hpp"
#include "Exception.hpp"

Constrainer::Constrainer(void)
{
    // nothing to do
}

Constrainer::~Constrainer(void)
{
    // nothing to do
}

int Constrainer::Constrain(const CoordinatePixel & input, CoordinatePixel & output) const
{
    throw Exception("Constrainer::Constrain(): shouldn't be called");
    return 0;
}
