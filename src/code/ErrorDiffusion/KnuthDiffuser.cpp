/*
  Knuth.cpp

  Li-Yi Wei
  04/30/2009

*/

#include "KnuthDiffuser.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

KnuthDiffuser::KnuthDiffuser(const vector<Pixel> & palette): Diffuser(palette)
{
    // nothing else to do
}

KnuthDiffuser::~KnuthDiffuser(void)
{
    // nothing else to do
}

int KnuthDiffuser::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    return _distributor.Coeff(input, coefficients);
}
