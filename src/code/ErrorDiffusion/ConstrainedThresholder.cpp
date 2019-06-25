/*
  ConstrainedThresholder.cpp

  Li-Yi Wei
  03/02/2010

*/

#include "ConstrainedThresholder.hpp"

ConstrainedThresholder::ConstrainedThresholder(const Thresholder & thresholder, const Constrainer & constrainer) : _thresholder(thresholder), _constrainer(constrainer)
{
    // nothing else to do
}

ConstrainedThresholder::~ConstrainedThresholder(void)
{
    // nothing else to do
}

int ConstrainedThresholder::Nearest(const CoordinatePixel & input, CoordinatePixel & output) const
{
    if(! _thresholder.Nearest(input, _intermediate))
    {
        return 0;
    }

    return _constrainer.Constrain(_intermediate, output);
}
