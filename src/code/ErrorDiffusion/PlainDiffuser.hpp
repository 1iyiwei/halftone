/*
  PlainDiffuser.hpp

  the plain diffuser that takes generic thresholder and distributor

  Li-Yi Wei
  03/03/2010

*/

#ifndef _PLAIN_DIFFUSER_HPP
#define _PLAIN_DIFFUSER_HPP

#include "Diffuser.hpp"
#include "Distributor.hpp"

class PlainDiffuser : public Diffuser
{
public:
    PlainDiffuser(const Thresholder & thresholder, const Distributor & distributor);
    virtual ~PlainDiffuser(void);

protected:
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

    const Distributor & _distributor;
};

#endif
