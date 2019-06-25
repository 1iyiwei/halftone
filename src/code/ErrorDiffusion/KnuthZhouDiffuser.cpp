/*
  KnuthZhou.cpp

  Li-Yi Wei
  05/02/2009

*/

#include "KnuthZhouDiffuser.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

KnuthZhouDiffuser::KnuthZhouDiffuser(const vector<Pixel> & palette, const Pixel & max_value): Diffuser(palette), _knuth(palette), _zhou(palette, max_value)
{
    // nothing else to do
}

KnuthZhouDiffuser::~KnuthZhouDiffuser(void)
{
    // nothing else to do
}

int KnuthZhouDiffuser::Nearest(const CoordinatePixel & input, CoordinatePixel & output) const
{
    return _zhou.Nearest(input, output);
}

int KnuthZhouDiffuser::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    return _knuth.Coeff(input, coefficients);
}
