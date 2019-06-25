/*
  ConstrainedThresholder.hpp

  constrain the output value for some of the pixels

  Li-Yi Wei
  03/02/2010

*/

#ifndef _CONSTRAINED_THRESHOLDER_HPP
#define _CONSTRAINED_THRESHOLDER_HPP

#include "Thresholder.hpp"
#include "Constrainer.hpp"

class ConstrainedThresholder : public Thresholder
{
public:
    // input the original (unconstrained) thresholder
    ConstrainedThresholder(const Thresholder & thresholder, const Constrainer & constrainer);
    virtual ~ConstrainedThresholder(void);

    int Nearest(const CoordinatePixel & input, CoordinatePixel & output) const;

protected:
    const Thresholder & _thresholder;
    const Constrainer & _constrainer;

    mutable CoordinatePixel _intermediate;
};

#endif
