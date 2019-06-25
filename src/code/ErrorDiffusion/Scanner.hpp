/*
  Scanner.hpp

  the base class for all scanners

  Li-Yi Wei
  04/24/2009

*/

#ifndef _SCANNER_HPP
#define _SCANNER_HPP

#include "CoordinatePixel.hpp"

class Scanner
{
public:

    typedef CoordinatePixel::Coordinate Coordinate;

    virtual ~Scanner(void) = 0;

    // return 1 if successful, 0 else
    virtual int Reset(void);

    // get the current coordinate value
    // return 1 if successful, 0 else
    virtual int GetCoordinate(Coordinate & coordinate) const;

    // transformation for the offset vectors, e.g. serpentine order
    virtual int GetOffset(const Coordinate & input, Coordinate & output) const;

    // move to the next value
    // return 1 if successful, 0 if the scanner stopped (already in final value)
    virtual int Next(void);
};

#endif
