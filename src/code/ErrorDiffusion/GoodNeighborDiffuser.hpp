/*
  GoodNeighborDiffuser.hpp

  the base class for all diffusers that are neigbhor-conscious
  i.e. pass errors only to not yet visited neighbors

  Li-Yi Wei
  04/30/2009

*/

#ifndef _GOOD_NEIGHBOR_DIFFUSER_HPP
#define _GOOD_NEIGHBOR_DIFFUSER_HPP

#include "Diffuser.hpp"
#include "Array.hpp"
#include "GoodNeighborDistributor.hpp"

class GoodNeighborDiffuser : public Diffuser
{
public:
    enum BoundaryCondition {BOUNDARY_NONE, BOUNDARY_TOROIDAL};

    GoodNeighborDiffuser(const Diffuser & diffuser, const Array<int> & occupancy_map, const BoundaryCondition & boundary_condition);
    virtual ~GoodNeighborDiffuser(void);

protected:
    int Nearest(const CoordinatePixel & query, CoordinatePixel & output) const;
    virtual int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

protected:
    const Diffuser & _diffuser;
    const Array<int> & _occupancy_map;
    const BoundaryCondition _boundary_condition;
};

#endif
