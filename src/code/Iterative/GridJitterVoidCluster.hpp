/*
  GridJitterVoidCluster.hpp

  use grid to find initial trough positions, followed by random jitter selection

  Li-Yi Wei
  05/15/2009

*/

#ifndef _GRID_JITTER_VOID_CLUSTER_HPP
#define _GRID_JITTER_VOID_CLUSTER_HPP

#include "GridContinuousVoidCluster.hpp"

class GridJitterVoidCluster : public GridContinuousVoidCluster
{
public:
    // candidate_size: the number of random jittered candidates to examine per grid trough point
    // jitter_radius: absolute value, not relative to the grid/domain size
    GridJitterVoidCluster(const vector<int> & grid_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size, const int candidate_size, const float jitter_radius);
    ~GridJitterVoidCluster(void);

protected:
    virtual int FindTroughs(const int peak_class_id, const int swap_size, vector<Sample::Position> & troughs) const;

    // random jittering in a spherical domain centered at input
    static int Jitter(const vector<double> & domain_size, const float jitter_radius, const Sample::Position & input, Sample::Position & output);
    
protected:
    const int _candidate_size;
    const float _jitter_radius;

    mutable vector<Datum> _trough_candidates;
    mutable vector<Sample> _trough_samples;
};

#endif
