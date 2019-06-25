/*
  GridRegularVoidCluster.hpp

  essentially DiscreteVoidCluster implemented under ContinuousVoidCluster

  Li-Yi Wei
  05/13/2009

*/

#ifndef _GRID_REGULAR_VOID_CLUSTER_HPP
#define _GRID_REGULAR_VOID_CLUSTER_HPP

#include "GridContinuousVoidCluster.hpp"

class GridRegularVoidCluster : public GridContinuousVoidCluster
{
public:
    GridRegularVoidCluster(const vector<int> & grid_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size);
    ~GridRegularVoidCluster(void);
};

#endif
