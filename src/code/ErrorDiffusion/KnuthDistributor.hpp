/*
  KnuthDistributor.hpp

  Li-Yi Wei
  03/02/2010

*/

#ifndef _KNUTH_DISTRIBUTOR_HPP
#define _KNUTH_DISTRIBUTOR_HPP

#include "Distributor.hpp"

class KnuthDistributor : public Distributor
{
public:
    KnuthDistributor(void);
    virtual ~KnuthDistributor(void);

// protected:
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;
};

#endif
