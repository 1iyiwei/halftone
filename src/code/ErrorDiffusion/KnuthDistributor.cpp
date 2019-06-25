/*
  KnuthDistributor.cpp

  Li-Yi Wei
  03/02/2010

*/

#include "KnuthDistributor.hpp"
#include "Exception.hpp"

KnuthDistributor::KnuthDistributor(void)
{
    // nothing else to do
}

KnuthDistributor::~KnuthDistributor(void)
{
    // nothing else to do
}

int KnuthDistributor::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    if(coefficients.size() != 8)
    {
        coefficients = vector<CoordinatePixel>(8);
    }

    for(unsigned int i = 0; i < coefficients.size(); i++)
    {
        coefficients[i] = input; // copy coordinate, and ensure same dim value

        if(coefficients[i].coordinate.Dimension() != 2) throw Exception("coefficients[i].coordinate.Dimension() != 2!");

        switch(i%4)
        {
        case 0:
            coefficients[i].coordinate[0] = 1;
            coefficients[i].coordinate[1] = 0;
            coefficients[i].value.Clear(1.0/6.0);
            break;

        case 1:
            coefficients[i].coordinate[0] = -1;
            coefficients[i].coordinate[1] = 0;
            coefficients[i].value.Clear(1.0/6.0);
            break;

        case 2:
            coefficients[i].coordinate[0] = 0;
            coefficients[i].coordinate[1] = 1;
            coefficients[i].value.Clear(1.0/6.0);
            break;

        case 3:
            coefficients[i].coordinate[0] = 0;
            coefficients[i].coordinate[1] = -1;
            coefficients[i].value.Clear(1.0/6.0);
            break;

        case 4:
            coefficients[i].coordinate[0] = -1;
            coefficients[i].coordinate[1] = -1;
            coefficients[i].value.Clear(1.0/12.0);
            break;

        case 5:
            coefficients[i].coordinate[0] = -1;
            coefficients[i].coordinate[1] = 1;
            coefficients[i].value.Clear(1.0/12.0);
            break;

        case 6:
            coefficients[i].coordinate[0] = 1;
            coefficients[i].coordinate[1] = -1;
            coefficients[i].value.Clear(1.0/12.0);
            break;

        case 7:
            coefficients[i].coordinate[0] = 1;
            coefficients[i].coordinate[1] = 1;
            coefficients[i].value.Clear(1.0/12.0);
            break;

        default:
            throw Exception("KnuthDiffuser::Coeff(): unanticipated error offset");
            break;
        }
    }

    // done
    return 1;
}
