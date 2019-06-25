/*
  GoodNeighborDistributor.cpp

  Li-Yi Wei
  03/03/2010

*/

#include "GoodNeighborDistributor.hpp"
#include "Exception.hpp"

GoodNeighborDistributor::GoodNeighborDistributor(const Distributor & distributor, const Array<int> & occupancy_map, const BoundaryCondition & boundary_condition) : _distributor(distributor), _occupancy_map(occupancy_map), _boundary_condition(boundary_condition)
{
    // nothing else to do
}

GoodNeighborDistributor::~GoodNeighborDistributor(void)
{
    // nothing else to do
}

int GoodNeighborDistributor::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    if(! _distributor.Coeff(input, coefficients))
    {
        return 0;
    }

    // zero out coeffients corresponding to neighbors already visited
    for(unsigned int i = 0; i < coefficients.size(); i++)
    {
        CoordinatePixel & coefficient = coefficients[i];

        vector<int> neighbor(coefficient.coordinate.Dimension());

        if((input.coordinate.Dimension() != coefficient.coordinate.Dimension()) || (input.coordinate.Dimension() != _occupancy_map.Dimension()))
        {
            throw Exception("GoodNeighborDiffuser::Error(): incompatible coordinate dimension");
        }

        for(unsigned int j = 0; j < neighbor.size(); j++)
        {
            neighbor[j] = input.coordinate[j] + coefficient.coordinate[j];

            if(_boundary_condition == BOUNDARY_TOROIDAL)
            {
                neighbor[j] = ((neighbor[j]%_occupancy_map.Size(j))+_occupancy_map.Size(j))%_occupancy_map.Size(j);
            }
        }

        int occupied = 0;
        if(!_occupancy_map.Get(neighbor, occupied)) occupied = 0;
        
        if(occupied)
        {
            coefficient.value.Clear(0);
        }
    }

    // and distribute the values proportionally to these not yet visited
    // simply by normalizing values of non-zero ones
    for(int j = 0; j < input.value.Dimension(); j++)
    {
        PixelType sum = 0;
        
        for(unsigned int i = 0; i < coefficients.size(); i++)
        {
            sum += coefficients[i].value[j];
        }
            
        if(sum > 0)
        {
            for(unsigned int i = 0; i < coefficients.size(); i++)
            {
                coefficients[i].value[j] /= sum;
            }
        }
    }

    // done
    return 1;
}
