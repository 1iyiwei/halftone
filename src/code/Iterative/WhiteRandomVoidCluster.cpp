/*
  WhiteRandomVoidCluster.cpp

  Li-Yi Wei
  05/18/2009
  
*/

#include <math.h>

#include "WhiteRandomVoidCluster.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Exception.hpp"

// #define _TROUGHS_CONSIDERING_PEAKS

WhiteRandomVoidCluster::WhiteRandomVoidCluster(const vector<double> & domain_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size, const int candidate_size): ContinuousVoidCluster(domain_size, class_weight, relative_splat_size, init_swap_size), _candidate_size(candidate_size), _trough_candidates(candidate_size), _trough_samples(candidate_size)
{
    for(unsigned int i = 0; i < _trough_candidates.size(); i++)
    {
        _trough_samples[i].position = Sample::Position(domain_size.size());
        _trough_candidates[i].sample = &_trough_samples[i];
    }
}

WhiteRandomVoidCluster::~WhiteRandomVoidCluster(void)
{
    // nothing else to do
}

int WhiteRandomVoidCluster::Value(Array<double> & value) const
{
    Array<Datum> grid(Convert(_domain_size));

    if(! VoidCluster::Scatter(_samples, _class_weight, 0, vector<int>(_num_samples.size(), 1), _relative_splat_size, 1, grid))
    {
        return 0;
    }

    return VoidCluster::Value(grid, value);
}

int WhiteRandomVoidCluster::Scatter(const vector<double> & domain_size, const vector<Sample> & input, const ClassWeight & class_weight, const int main_class_id, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first)
{
    // potential peaks
    if(! ContinuousVoidCluster::Scatter(domain_size, input, class_weight, main_class_id, class_selection, relative_splat_size, clear_first, _bag))
    {
        return 0;
    }

    // done
    return 1;
}

int WhiteRandomVoidCluster::FindTroughs(const int peak_class_id, const int swap_size, vector<Sample::Position> & troughs) const
{
#ifdef _TROUGHS_CONSIDERING_PEAKS
    vector<Datum> peaks(swap_size);
    if(! FindPeaks(_bag, peak_class_id, swap_size, peaks))
    {
        return 0;
    }

    if(peaks.size() != swap_size)
    {
        throw Exception("WhiteRandomVoidCluster::FindTroughs(): peaks.size() != swap_size");
        return 0;
    }
#endif

    troughs = vector<Sample::Position>(swap_size);
    
    for(unsigned int which_trough = 0; which_trough < troughs.size(); which_trough++)
    {
        Sample::Position & trough = troughs[which_trough];

        // find jittered positions
        for(unsigned int i = 0; i < _trough_candidates.size(); i++)
        {
            if(!Dart(_domain_size, const_cast<Sample *>(_trough_candidates[i].sample)->position))
            {
                return 0;
            }
        }

#ifdef _TROUGHS_CONSIDERING_PEAKS
        if(_trough_candidates.size() > 1)
        {
            const_cast<Sample *>(_trough_candidates[0].sample)->position = peaks[which_trough].sample->position;
        }
#endif

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

int WhiteRandomVoidCluster::Dart(const vector<double> & domain_size, Sample::Position & output)
{
    if(domain_size.size() != output.Dimension())
    {
        output = Sample::Position(domain_size.size());
    }

    // white random
    for(int i = 0; i < output.Dimension(); i++)
    {
        output[i] = Random::UniformRandom()*domain_size[i];
    }

    // done
    return 1;
}
