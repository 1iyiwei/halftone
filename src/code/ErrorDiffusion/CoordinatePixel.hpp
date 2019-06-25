/*
  CoordinatePixel.hpp

  pixel with coordinate information for error diffusion

  Li-Yi Wei
  04/22/2009

*/

#ifndef _COORDINATE_PIXEL_HPP
#define _COORDINATE_PIXEL_HPP

#include "Pixel.hpp"
#include "Coordinate.hpp"

class CoordinatePixel
{
public:
    typedef CoordinateT<int> Coordinate;
    Coordinate coordinate;
    Pixel value;
};

#endif
