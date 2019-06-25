/*
  GoodNeighborDistributor.hpp

  Li-Yi Wei
  03/03/2010

*/

#ifndef _GOOD_NEIGHBOR_DISTRIBUTOR_HPP
#define _GOOD_NEIGHBOR_DISTRIBUTOR_HPP

#include "Distributor.hpp"
#include "Array.hpp"

class GoodNeighborDistributor : public Distributor
{
public:
    enum BoundaryCondition {BOUNDARY_NONE, BOUNDARY_TOROIDAL};

    GoodNeighborDistributor(const Distributor & distributor, const Array<int> & occupancy_map, const BoundaryCondition & boundary_condition);
    virtual ~GoodNeighborDistributor(void);

// protected:
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

protected:
    const Distributor & _distributor;
    const Array<int> & _occupancy_map;
    const BoundaryCondition _boundary_condition;
};

#endif
