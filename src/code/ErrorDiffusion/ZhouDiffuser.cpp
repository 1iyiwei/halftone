/*
  Zhou.cpp

  Li-Yi Wei
  04/23/2009

*/

#include "ZhouDiffuser.hpp"

#include "Exception.hpp"
#include "Utility.hpp"
#include "Random.hpp"

ZhouDiffuser::ZhouDiffuser(const vector<Pixel> & palette, const Pixel & max_value): Diffuser(palette), _zhou_thresholder(palette, max_value), _thresholder(_zhou_thresholder), _distributor(max_value)
{
    // nothing else to do
}

ZhouDiffuser::~ZhouDiffuser(void)
{
    // nothing else to do
}

int ZhouDiffuser::Nearest(const CoordinatePixel & input, CoordinatePixel & output) const
{
    return _thresholder.Nearest(input, output);
}

int ZhouDiffuser::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    return _distributor.Coeff(input, coefficients);
}
