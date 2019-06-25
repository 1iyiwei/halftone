/*
  KnuthZhouDiffuser.hpp

  the symmetric diffuser Knuth used in his 1987 Dot Diffusion paper

  Li-Yi Wei
  04/30/2009

*/

#ifndef _KNUTH_ZHOU_DIFFUSER_HPP
#define _KNUTH_ZHOU_DIFFUSER_HPP

#include "Diffuser.hpp"
#include "KnuthDiffuser.hpp"
#include "ZhouDiffuser.hpp"

class KnuthZhouDiffuser : public Diffuser
{
public:
    KnuthZhouDiffuser(const vector<Pixel> & palette, const Pixel & max_value);
    ~KnuthZhouDiffuser(void);

protected:
    int Nearest(const CoordinatePixel & input, CoordinatePixel & output) const;

    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

private:
    KnuthDiffuser _knuth;
    ZhouDiffuser _zhou;
};

#endif
