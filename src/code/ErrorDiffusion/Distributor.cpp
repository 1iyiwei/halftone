/*
  Distributor.cpp

  Li-Yi Wei
  03/02/2010

*/

#include "Distributor.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

Distributor::Distributor(void)
{
    // nothing else to do
}

Distributor::~Distributor(void)
{
    // nothing else to do
}
    
int Distributor::Error(const CoordinatePixel & input, const CoordinatePixel & output, vector<CoordinatePixel> & errors) const
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

int Distributor::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    // shouldn't be called
    throw Exception("Distributor::Coeff() should not be called");
    return 0;
}
