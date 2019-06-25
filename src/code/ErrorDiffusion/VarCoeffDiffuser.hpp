/*
  VarCoeffDiffuser.hpp

  Victor Ostromoukhov's SIGGRAPH 2001 algorithm

  Li-Yi Wei
  04/26/2009

*/

#ifndef _VAR_COEFF_DIFFUSER_HPP
#define _VAR_COEFF_DIFFUSER_HPP

#include "Diffuser.hpp"
#include "VarCoeffDistributor.hpp"

class VarCoeffDiffuser : public Diffuser
{
public:
    VarCoeffDiffuser(const vector<Pixel> & palette, const Pixel & max_value);
    virtual ~VarCoeffDiffuser(void);

protected:
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;
    const VarCoeffDistributor _distributor;
};

#endif
