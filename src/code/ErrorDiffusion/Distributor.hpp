/*
  Distributor.hpp

  the base class of all error diffusion distributors

  Li-Yi Wei
  03/02/2010

*/

#ifndef _DISTRIBUTOR_HPP
#define _DISTRIBUTOR_HPP

#include <vector>
using namespace std;

#include "CoordinatePixel.hpp"

class Distributor
{
public:
    Distributor(void);
    virtual ~Distributor(void) = 0;
    
    // compute the error values to be distributed to neighbors
    virtual int Error(const CoordinatePixel & input, const CoordinatePixel & output, vector<CoordinatePixel> & errors) const;

// protected:

    // get the pristine diffusion coefficients without considering neighbors
    // offspring classes will need to flesh out this
    // (use Coeff because it is short and sounds like a verb)
    virtual int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

protected:
    mutable vector<CoordinatePixel> _coefficients;
};

#endif
