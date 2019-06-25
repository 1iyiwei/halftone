/*
  ArrayContainer.cpp

  Li-Yi Wei
  03/02/2010

*/

#include "ArrayConstrainer.hpp"

ArrayConstrainer::ArrayConstrainer(const Array<ConstrainedPixel> & array) : _array(array)
{
    // nothing else to do
}

ArrayConstrainer::~ArrayConstrainer(void)
{
    // nothing else to do
}

int ArrayConstrainer::Constrain(const CoordinatePixel & input, CoordinatePixel & output) const
{
    if(_index.size() != input.coordinate.Dimension())
    {
        _index = vector<int>(input.coordinate.Dimension());
    }

    for(unsigned int i = 0; i < _index.size(); i++)
    {
        _index[i] = input.coordinate[i];
    }

    const int status = _array.Get(_index, _pixel);

    output = input;

    if(status && _pixel.constrained)
    {
        output.value = _pixel.value;
    }

    return status;
}
