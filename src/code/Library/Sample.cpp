/*
  Sample.cpp

  Li-Yi Wei
  06/28/2007

*/

#include "Sample.hpp"

Sample::Sample(void) : id(-1), value(1.0), r(0.0)
{
    // nothing to do
}

Sample::Sample(const int dim) : id(-1), value(1.0), r(0.0), coordinate(dim)
{
    // nothing to do
}

Sample::~Sample(void)
{
    // nothing to do
}
