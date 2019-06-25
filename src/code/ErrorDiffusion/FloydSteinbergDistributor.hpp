/*
  FloydSteinbergDistributor.hpp

  Li-Yi Wei
  03/02/2010

*/

#ifndef _FLOYD_STEINBERG_DISTRIBUTOR_HPP
#define _FLOYD_STEINBERG_DISTRIBUTOR_HPP

#include "Distributor.hpp"

class FloydSteinbergDistributor : public Distributor
{
public:
    FloydSteinbergDistributor(void);
    virtual ~FloydSteinbergDistributor(void);

// protected:
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;
};

#endif
