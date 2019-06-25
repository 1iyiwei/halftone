/*
  ZhouDistributor.hpp

  Li-Yi Wei
  03/02/2010

*/

#ifndef _ZHOU_DISTRIBUTOR_HPP
#define _ZHOU_DISTRIBUTOR_HPP

#include "VarCoeffDistributor.hpp"

class ZhouDistributor : public VarCoeffDistributor
{
public:
    ZhouDistributor(const Pixel & max_value);
    virtual ~ZhouDistributor(void);

private: 
    struct IndexedThreeCoeffsInt
    {
        int index;
        ThreeCoeffsInt coeffs;
    };

    static const int _MODIFIED_COEFFS_TABLE_SIZE;
    static const IndexedThreeCoeffsInt _MODIFIED_COEFFS_TABLE[];
};

#endif
