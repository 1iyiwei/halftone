/*
  Thresholder.hpp

  the part of Diffuser that determines the output value via thresholding

  Li-Yi Wei
  03/01/2010

*/

#ifndef _THRESHOLDER_HPP
#define _THRESHOLDER_HPP

#include <vector>
using namespace std;

#include "CoordinatePixel.hpp"

class Thresholder
{
public:
    Thresholder(void);
    Thresholder(const vector<Pixel> & palette);
    virtual ~Thresholder(void) = 0;

    // find the nearest entry in the palette for the query
    virtual int Nearest(const CoordinatePixel & input, CoordinatePixel & output) const;

protected:
    virtual int Nearest(const Pixel & query, const vector<Pixel> & palette) const;

protected:
    const vector<Pixel> _palette;
};

#endif

