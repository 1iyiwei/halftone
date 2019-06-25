/*
  Diffuser.hpp

  the base class for the core site operator for error diffusion

  Li-Yi Wei
  04/22/2009

*/

#ifndef _DIFFUSER_HPP
#define _DIFFUSER_HPP

#include <vector>
using namespace std;

#include "Pixel.hpp"
#include "CoordinatePixel.hpp"

#include "PlainThresholder.hpp"

class Diffuser
{
    friend class GoodNeighborDiffuser;

public:
    Diffuser(const Thresholder & thresholder);
    Diffuser(const vector<Pixel> & palette);
    virtual ~Diffuser(void) = 0;
  
    // core operation
    // the input/output coordinates are in global frame
    // but the errors coordinates are offsets
    // return 1 if successful, 0 if error
    virtual int Diffuse(const CoordinatePixel & input, CoordinatePixel & output, vector<CoordinatePixel> & errors) const;

protected:
    const PlainThresholder _plain_thresholder;
    const Thresholder & _thresholder;

    // the two component functions for Diffuse()

    // find the nearest entry in the palette for the query
    virtual int Nearest(const CoordinatePixel & input, CoordinatePixel & output) const;

    // compute the error values to be distributed to neighbors
    virtual int Error(const CoordinatePixel & input, const CoordinatePixel & output, vector<CoordinatePixel> & errors) const;

    // get the pristine diffusion coefficients without considering neighbors
    // offspring classes will need to flesh out this
    // (use Coeff because it is short and sounds like a verb)
    virtual int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

protected:
    mutable vector<CoordinatePixel> _coefficients;
};

#endif
