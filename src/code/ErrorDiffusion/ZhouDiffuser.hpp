/*
  ZhouDiffuser.hpp

  BingFeng Zhou's SIGGRAPH 2003 algorithm

  Li-Yi Wei
  04/27/2009

*/

#ifndef _ZHOU_DIFFUSER_HPP
#define _ZHOU_DIFFUSER_HPP

#include "Diffuser.hpp"
#include "ZhouThresholder.hpp"
#include "ZhouDistributor.hpp"

class ZhouDiffuser : public Diffuser
{
    friend class KnuthZhouDiffuser;

public:
    ZhouDiffuser(const vector<Pixel> & palette, const Pixel & max_value);
    virtual ~ZhouDiffuser(void);

protected:
    int Nearest(const CoordinatePixel & input, CoordinatePixel & output) const;
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

protected:
    const ZhouThresholder _zhou_thresholder;
    const Thresholder & _thresholder;
    const ZhouDistributor _distributor;
};

#endif
