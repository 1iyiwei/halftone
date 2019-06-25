/*
  VarCoeff.cpp

  Li-Yi Wei
  04/26/2009

*/

#include <algorithm>
using namespace std;

#include <math.h>

#include "VarCoeffDiffuser.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

VarCoeffDiffuser::VarCoeffDiffuser(const vector<Pixel> & palette, const Pixel & max_value): Diffuser(palette), _distributor(max_value)
{
    for(unsigned int i = 0; i < palette.size(); i++)
    {
        if(palette[i].Dimension() != max_value.Dimension()) throw Exception("VarCoeffDiffuser::VarCoeffDiffuser(): palette mismatch");
    }
}

VarCoeffDiffuser::~VarCoeffDiffuser(void)
{
    // nothing else to do
}
 
int VarCoeffDiffuser::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    return _distributor.Coeff(input, coefficients);
}
