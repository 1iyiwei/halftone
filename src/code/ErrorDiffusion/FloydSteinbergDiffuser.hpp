/*
  FloydSteinbergDiffuser.hpp

  standard traditinal FS error diffusion

  Li-Yi Wei
  04/23/2009

*/

#ifndef _FLOYD_STEINBERG_DIFFUSER_HPP
#define _FLOYD_STEINBERG_DIFFUSER_HPP

#include "Diffuser.hpp"
#include "FloydSteinbergDistributor.hpp"

class FloydSteinbergDiffuser : public Diffuser
{
public:
    FloydSteinbergDiffuser(const vector<Pixel> & palette);
    ~FloydSteinbergDiffuser(void);

protected:
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

protected:
    const FloydSteinbergDistributor _distributor;
};

#endif
