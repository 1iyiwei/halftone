/*
  Plain.cpp

  Li-Yi Wei
  03/03/2010

*/

#include <algorithm>
using namespace std;

#include <math.h>

#include "PlainDiffuser.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

PlainDiffuser::PlainDiffuser(const Thresholder & thresholder, const Distributor & distributor) : Diffuser(thresholder), _distributor(distributor)
{
    // nothing else to do
}

PlainDiffuser::~PlainDiffuser(void)
{
    // nothing else to do
}
 
int PlainDiffuser::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    return _distributor.Coeff(input, coefficients);
}
