/*
  Thresholder.cpp

  Li-Yi Wei
  03/01/2010

*/

#include "Thresholder.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

Thresholder::Thresholder(void): _palette(vector<Pixel>(1))
{
    // nothing else to do
}

Thresholder::Thresholder(const vector<Pixel> & palette): _palette(palette)
{
    if(_palette.size() <= 0) throw Exception("Thresholder::Thresholder(): palette size must be > 0");
}

Thresholder::~Thresholder(void)
{
    // nothing to do
}

int Thresholder::Nearest(const CoordinatePixel & input, CoordinatePixel & output) const
{
    const int min_index = Nearest(input.value, _palette);

    output.coordinate = input.coordinate;
    output.value = _palette[min_index];

    return 1;
}

int Thresholder::Nearest(const Pixel & query, const vector<Pixel> & palette) const
{
    if(palette.size() <= 0)
    {
        throw Exception("Thresholder::Nearest(): empty palette");
        return -1;
    }

    int min_index = 0;
    float min_distance = Utility::Distance2(query, palette[min_index]);

    for(unsigned int i = min_index+1; i < palette.size(); i++)
    {
        const float distance = Utility::Distance2(query, palette[i]);

        if(distance < min_distance)
        {
            min_index = i;
            min_distance = distance;
        }
    }

    return min_index;
}
