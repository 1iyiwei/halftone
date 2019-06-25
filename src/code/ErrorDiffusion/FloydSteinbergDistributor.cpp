/*
  FloydSteinbergDistributor.cpp

  Li-Yi Wei
  03/02/2010

*/

#include "FloydSteinbergDistributor.hpp"
#include "Exception.hpp"

FloydSteinbergDistributor::FloydSteinbergDistributor(void)
{
    // nothing else to do
}

FloydSteinbergDistributor::~FloydSteinbergDistributor(void)
{
    // nothing else to do
}

int FloydSteinbergDistributor::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    if(coefficients.size() != 4)
    {
        coefficients = vector<CoordinatePixel>(4);
    }

    for(unsigned int i = 0; i < coefficients.size(); i++)
    {
        coefficients[i] = input; // copy coordinate, and ensure same dim value

        if(coefficients[i].coordinate.Dimension() < 2) throw Exception("coefficients[i].coordinate.Dimension() < 2!");

        switch(i%4)
        {
        case 0:
            coefficients[i].coordinate[0] = 1;
            coefficients[i].coordinate[1] = 0;
            coefficients[i].value.Clear(7.0/16.0);
            break;

        case 1:
            coefficients[i].coordinate[0] = -1;
            coefficients[i].coordinate[1] = 1;
            coefficients[i].value.Clear(3.0/16.0);
            break;

        case 2:
            coefficients[i].coordinate[0] = 0;
            coefficients[i].coordinate[1] = 1;
            coefficients[i].value.Clear(5.0/16.0);
            break;

        case 3:
            coefficients[i].coordinate[0] = 1;
            coefficients[i].coordinate[1] = 1;
            coefficients[i].value.Clear(1.0/16.0);
            break;

        default:
            throw Exception("FloydSteinbergDiffuser::Coeff(): unanticipated error offset");
            break;
        }
    }

    // done
    return 1;
}
