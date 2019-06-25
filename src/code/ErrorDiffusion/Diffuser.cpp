/*
  Diffuser.cpp

  Li-Yi Wei
  04/22/2009

*/

#include "Diffuser.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

Diffuser::Diffuser(const Thresholder & thresholder) : _plain_thresholder(vector<Pixel>(1)), _thresholder(thresholder)
{
    // nothing else to do
}

Diffuser::Diffuser(const vector<Pixel> & palette): _plain_thresholder(palette), _thresholder(_plain_thresholder)
{
    // nothing else to do
}

Diffuser::~Diffuser(void)
{
    // nothing to do
}

int Diffuser::Diffuse(const CoordinatePixel & input, CoordinatePixel & output, vector<CoordinatePixel> & errors) const
{
    if(! Nearest(input, output)) return 0;
    return Error(input, output, errors);
}

int Diffuser::Nearest(const CoordinatePixel & input, CoordinatePixel & output) const
{
    return _thresholder.Nearest(input, output);
}

int Diffuser::Error(const CoordinatePixel & input, const CoordinatePixel & output, vector<CoordinatePixel> & errors) const
{
    const Pixel error = Utility::Diff(input.value, output.value);

    if(! Coeff(input, _coefficients))
    {
        return 0;
    }

    errors = _coefficients; // ensure same size and get coordinate

    for(unsigned int i = 0; i < errors.size(); i++)
    {
        errors[i].value = input.value; // ensure dimension compatibility

        if(errors[i].value.Dimension() != _coefficients[i].value.Dimension())
        {
            throw Exception("Diffuser::Error(): dimension mismatch!");
        }

        for(int j = 0; j < errors[i].value.Dimension(); j++)
        {
            errors[i].value[j] = error[j]*_coefficients[i].value[j];
        }
    }

    // done
    return 1;
}

int Diffuser::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    // shouldn't be called
    throw Exception("Diffuser::Coeff() should not be called");
    return 0;
}
