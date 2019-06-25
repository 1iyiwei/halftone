/*
  ArrayConstrainer.hpp

  obtain constrain information via a pixel array

  Li-Yi Wei
  03/02/2010

*/

#ifndef _ARRAY_CONSTRAINER_HPP
#define _ARRAY_CONSTRAINER_HPP

#include "Constrainer.hpp"
#include "Array.hpp"

class ArrayConstrainer : public Constrainer
{
public:
    struct ConstrainedPixel
    {
        ConstrainedPixel(void): constrained(0) {};

        Pixel value;
        bool constrained;
    };

public:
    ArrayConstrainer(const Array<ConstrainedPixel> & array);
    virtual ~ArrayConstrainer(void);

    int Constrain(const CoordinatePixel & input, CoordinatePixel & output) const;

protected:
    const Array<ConstrainedPixel> & _array;
    mutable vector<int> _index;
    mutable ConstrainedPixel _pixel;
};

#endif

