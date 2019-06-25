/*
  Constrainer.hpp

  the base class for all constrainers for pixel output

  Li-Yi Wei
  03/02/2010

*/

#ifndef _CONSTRAINER_HPP
#define _CONSTRAINER_HPP

#include "CoordinatePixel.hpp"

class Constrainer
{
public:
    Constrainer(void);
    virtual ~Constrainer(void) = 0;

    virtual int Constrain(const CoordinatePixel & input, CoordinatePixel & output) const;
};

#endif
