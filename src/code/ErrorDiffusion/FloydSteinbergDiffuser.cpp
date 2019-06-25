/*
  FloydSteinberg.cpp

  Li-Yi Wei
  04/23/2009

*/

#include "FloydSteinbergDiffuser.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

FloydSteinbergDiffuser::FloydSteinbergDiffuser(const vector<Pixel> & palette): Diffuser(palette)
{
    // nothng else to do
}

FloydSteinbergDiffuser::~FloydSteinbergDiffuser(void)
{
    // nothing else to do
}

int FloydSteinbergDiffuser::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    return _distributor.Coeff(input, coefficients);
}
