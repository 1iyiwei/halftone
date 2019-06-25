/*
  GridJitterVoidCluster.cpp

  Li-Yi Wei
  05/15/2009

*/

#include <math.h>

#include "GridJitterVoidCluster.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Exception.hpp"
  
GridJitterVoidCluster::GridJitterVoidCluster(const vector<int> & grid_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size, const int candidate_size, const float jitter_radius) : GridContinuousVoidCluster(grid_size, class_weight, relative_splat_size, init_swap_size), _candidate_size(candidate_size), _jitter_radius(jitter_radius), _trough_candidates(candidate_size), _trough_samples(candidate_size)
{
    for(unsigned int i = 0; i < _trough_candidates.size(); i++)
    {
        _trough_candidates[i].sample = &_trough_samples[i];
    }
}

GridJitterVoidCluster::~GridJitterVoidCluster(void)
{
    // nothing else to do
}

int GridJitterVoidCluster::FindTroughs(const int peak_class_id, const int swap_size, vector<Sample::Position> & troughs) const
{
    if(! GridContinuousVoidCluster::FindTroughs(peak_class_id, swap_size, troughs))
    {
        return 0;
    }

    for(unsigned int which_trough = 0; which_trough < troughs.size(); which_trough++)
    {
        Sample::Position & trough = troughs[which_trough];

        // find jittered positions
        for(unsigned int i = 0; i < _trough_candidates.size(); i++)
        {
            if(!Jitter(_domain_size, _jitter_radius, trough, const_cast<Sample *>(_trough_candidates[i].sample)->position))
            {
                return 0;
            }
        }

        // scatter
        if(!ContinuousVoidCluster::Scatter(_domain_size, _samples, _class_weight, peak_class_id, vector<int>(_num_samples.size(), 1), _relative_splat_size, 1, _trough_candidates))
        {
            return 0;
        }

        // find minimum
        int min_index = -1;
        double min_value = 1.0/sin(0.0);

        for(unsigned int i = 0; i < _trough_candidates.size(); i++)
        {
            if(_trough_candidates[i].value < min_value)
            {
                min_value = _trough_candidates[i].value;
                min_index = i;
            }
        }

        if((min_index < 0) || (min_index >= _trough_candidates.size()))
        {
            return 0;
        }

        trough = _trough_candidates[min_index].sample->position;
    }

    // done
    return 1;
}

int GridJitterVoidCluster::Jitter(const vector<double> & domain_size, const float jitter_radius, const Sample::Position & input, Sample::Position & output)
{
    if(domain_size.size() != input.Dimension())
    {
        return 0;
    }

    output = input;

    // gaussian random
    // could I produce a multi-dimensional Gaussian random by components?
    for(int i = 0; i < output.Dimension(); i++)
    {
        output[i] += Random::GaussianRandom()*jitter_radius;
    }

    // toroidally wrap back into grid
    for(int i = 0; i < output.Dimension(); i++)
    {
        output[i] = mod(output[i], domain_size[i]);
    }

    // done
    return 1;
}
