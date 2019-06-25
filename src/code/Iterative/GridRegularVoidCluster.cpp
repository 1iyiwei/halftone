/*
  GridRegularVoidCluster.cpp

  Li-Yi Wei
  05/13/2009

*/

#include "GridRegularVoidCluster.hpp"
#include "Exception.hpp"
  
GridRegularVoidCluster::GridRegularVoidCluster(const vector<int> & grid_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size) : GridContinuousVoidCluster(grid_size, class_weight, relative_splat_size, init_swap_size)
{
    // nothing else to do
}

GridRegularVoidCluster::~GridRegularVoidCluster(void)
{
    // nothing else to do
}
