/*
  GridContinuousVoidCluster.hpp

  the base class for all continous void-cluster that uses grid for scattering

  Li-Yi Wei
  05/15/2009

*/

#ifndef _GRID_CONTINUOUS_VOID_CLUSTER_HPP
#define _GRID_CONTINUOUS_VOID_CLUSTER_HPP

#include "ContinuousVoidCluster.hpp"

class GridContinuousVoidCluster : public ContinuousVoidCluster
{
public:
    GridContinuousVoidCluster(const vector<int> & grid_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size);
    virtual ~GridContinuousVoidCluster(void) = 0;

    virtual int Value(Array<double> & value) const;

protected:
    virtual int Scatter(const vector<double> & domain_size, const vector<Sample> & input, const ClassWeight & class_weight, const int main_class_id, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first);

    virtual int FindTroughs(const int peak_class_id, const int swap_size, vector<Sample::Position> & troughs) const;

protected:
    Array<Datum> _grid;
};

#endif
