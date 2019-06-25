/*
  KnuthDiffuser.hpp

  the symmetric diffuser Knuth used in his 1987 Dot Diffusion paper

  Li-Yi Wei
  04/30/2009

*/

#ifndef _KNUTH_DIFFUSER_HPP
#define _KNUTH_DIFFUSER_HPP

#include "Diffuser.hpp"
#include "KnuthDistributor.hpp"

class KnuthDiffuser : public Diffuser
{
    friend class KnuthZhouDiffuser;

public:
    KnuthDiffuser(const vector<Pixel> & palette);
    ~KnuthDiffuser(void);

protected:
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

    const KnuthDistributor _distributor;
};

#endif
