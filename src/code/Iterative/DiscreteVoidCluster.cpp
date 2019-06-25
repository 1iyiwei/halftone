/*
  DiscreteVoidCluster.cpp

  Li-Yi Wei
  05/03/2009

*/

#include <algorithm>
using namespace std;

#include <math.h>

#include "DiscreteVoidCluster.hpp"
#include "SequentialCounter.hpp"
#include "OnionLayerCounter.hpp"
#include "RandomCounter.hpp"
#include "Math.hpp"
#include "Exception.hpp"

//#define EFFICIENT_SCATTERING

DiscreteVoidCluster::DiscreteVoidCluster(const vector<int> & grid_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size) : VoidCluster(class_weight, relative_splat_size, init_swap_size), _grid(grid_size)
{
    // nothing else to do
}

DiscreteVoidCluster::~DiscreteVoidCluster(void)
{
    // nothing else to do
}

VoidCluster::RunStatus DiscreteVoidCluster::Init(const vector<Sample> & input)
{
    RunStatus status = FAILURE;

    if((status = VoidCluster::Init(input)) != OK)
    {
        return status;
    }

    // the samples should always register properly with the grid
    if(! Scatter(_samples, _grid))
    {
        throw Exception("DiscreteVoidCluster::Init(): error in scatter samples");
        return FAILURE;
    }

    return OK;
}

VoidCluster::RunStatus DiscreteVoidCluster::Next(vector<Sample> & output)
{
    // go through each class, making all unions containing it happy
    vector<int> grid_size; _grid.Size(grid_size);
    const int num_classes = _num_samples.size();

    vector<int> class_selection(num_classes);

    for(int which_class = 0; which_class < num_classes; which_class++)
    {
        // scatter
        class_selection = vector<int>(num_classes, 1);
        if(! VoidCluster::Scatter(_samples, _class_weight, which_class, class_selection, _relative_splat_size, 1, _grid))
        {
            throw Exception("DiscreteVoidCluster::Next(): cannot scatter initial samples");
            return FAILURE;
        }

        // find replacements
        vector< vector<int> > peaks, troughs;

        if(! Find(_grid, which_class, _swap_size, peaks, troughs))
        {
            throw Exception("DiscreteVoidCluster::Next(): error in finding peaks and troughs");
            return FAILURE;
        }
        
        // energy measurement
        const double energy_old = Energy(_grid);

#ifdef EFFICIENT_SCATTERING
        // scatter, remove value from which_class
        class_selection = vector<int>(num_classes, 0);
        class_selection[which_class] = -1;
        if(! VoidCluster::Scatter(_samples, _class_weight, which_class, class_selection, _relative_splat_size, 0, _grid))
        {
            throw Exception("DiscreteVoidCluster::Next(): cannot scatter-remove target_class samples");
            return FAILURE;
        }
#endif

        // perform replacement
        if(! Swap(peaks, troughs, _grid))
        {
            throw Exception("DiscreteVoidCluster::Swap(): error in swapping peaks and troughs");
            return FAILURE;
        }
        
#ifdef EFFICIENT_SCATTERING
        // scatter, add value from which_class
        class_selection = vector<int>(num_classes, 0);
        class_selection[which_class] = 1;
        if(! VoidCluster::Scatter(_samples, _class_weight, which_class, class_selection, _relative_splat_size, 0, _grid))
        {
            throw Exception("DiscreteVoidCluster::Next(): cannot scatter-add target_class samples");
            return FAILURE;
        }
#else
        class_selection = vector<int>(num_classes, 1);
        if(! VoidCluster::Scatter(_samples, _class_weight, which_class, class_selection, _relative_splat_size, 1, _grid))
        {
            throw Exception("DiscreteVoidCluster::Next(): cannot scatter initial samples");
            return FAILURE;
        } 
#endif
        // gather samples
        output = _samples;

        // energy measurement and update swap size if necessary
        const double energy_new = Energy(_grid);

        // cerr << "energy old and new: " << energy_old << " " << energy_new << endl;
        if(energy_new > energy_old)
        {
            if(_swap_size > 1)
            {
                _swap_size /= 2;
            }
            else
            {
                return DONE;
            }
        }
    }

    // done
    return OK;
}

int DiscreteVoidCluster::Scatter(const vector<Sample> & samples, Array<Datum> & grid)
{
    Datum empty;
    grid.Clear(empty);

    vector<int> index(grid.Dimension());

    for(unsigned int which_sample = 0; which_sample < samples.size(); which_sample++)
    {
        const Sample & sample = samples[which_sample];

        if(index.size() != sample.position.Dimension())
        {
            throw Exception("DiscreteVoidCluster::Scatter(): dimension mismatch");
            return 0;
        }

        for(unsigned int i = 0; i < index.size(); i++)
        {
            index[i] = static_cast<int>(sample.position[i]);
        }

        if(! grid.Get(index, empty))
        {
            throw Exception("DiscreteVoidCluster::Scatter(): cannot get cell");
            return 0;
        }

        if(empty.sample)
        {
            throw Exception("DiscreteVoidCluster::Scatter(): cell not empty");
            return 0;
        }
        
        empty.value = 0; empty.sample = &sample;

        if(! grid.Put(index, empty))
        {
            throw Exception("DiscreteVoidCluster::Scatter(): cannot put cell");
            return 0;
        }
    }

    // done
    return 1;
}

int DiscreteVoidCluster::Find(const Array<Datum> & grid, const int peak_class_id, const int swap_size, vector< vector<int> > & peaks_out, vector< vector<int> > & troughs_out)
{
#if 1
    if(! VoidCluster::FindPeaks(grid, peak_class_id, swap_size, peaks_out))
    {
        return 0;
    }

    if(! VoidCluster::FindTroughs(grid, swap_size, troughs_out))
    {
        return 0;
    }

    // trim
    while(peaks_out.size() > troughs_out.size())
    {
        peaks_out.pop_back();
    }
    while(troughs_out.size() > peaks_out.size())
    {
        troughs_out.pop_back();
    }

    return peaks_out.size();
#else
    vector<IndexDatum> peaks, troughs;
    
    const int dimension = grid.Dimension();
    vector<int> grid_size(dimension); grid.Size(grid_size);
    
    vector<int> counter_max(grid_size);
    for(unsigned int i = 0; i < counter_max.size(); i++)
    {
        counter_max[i] = grid_size[i]-1;
    }

    RandomCounter counter(dimension, vector<int>(dimension, 0), counter_max);
    counter.Reset();
    IndexDatum index_datum;

    do
    {
        // assuming swap size is reasonably small, so use search instead of sort

        counter.Get(index_datum.index);

        if(! grid.Get(index_datum.index, index_datum.data))
        {
            throw Exception("DiscreteVoidCluster::Find(): cannot get grid data");
            return 0;
        }

        if(!index_datum.data.sample)
        {
            // potential trough
            int location = -1;
            for(unsigned int i = 0; i < troughs.size(); i++)
            {
                if(troughs[i].data.value > index_datum.data.value)
                {
                    location = i; break;
                }
            }

            // insert before
            troughs.insert(location >= 0 ? troughs.begin()+location : troughs.end(), index_datum);

            if(troughs.size() > swap_size) troughs.pop_back();
        }
        else if(index_datum.data.sample->id == peak_class_id)
        {
            // potential peak
            int location = -1;
            for(unsigned int i = 0; i < peaks.size(); i++)
            {
                if(peaks[i].data.value < index_datum.data.value)
                {
                    location = i; break;
                }
            }

            // insert before
            peaks.insert(location >= 0 ? peaks.begin()+location : peaks.end(), index_datum);

            if(peaks.size() > swap_size) peaks.pop_back();
        }
        else
        {
            // neither peak nor trough
            // so do nothing
        }
    }
    while(counter.Next());
    //while(counter.Next() && ((peaks.size() < swap_size) || (troughs.size() < swap_size))); // this is wrong

    const int final_swap_size = min(peaks.size(), troughs.size());

    peaks_out = troughs_out = vector< vector<int> >(final_swap_size);

    for(unsigned int i = 0; i < peaks_out.size(); i++)
    {
        peaks_out[i] = peaks[i].index;
    } 
    for(unsigned int i = 0; i < troughs_out.size(); i++)
    {
        troughs_out[i] = troughs[i].index;
    }

    // done
    return final_swap_size;

#endif
}

int DiscreteVoidCluster::Swap(const vector< vector<int> > & peaks, const vector< vector<int> > & troughs, Array<Datum> & grid)
{
    if(peaks.size() != troughs.size())
    {
        return 0;
    }

    Datum peak, trough;

    for(unsigned which_entry = 0; which_entry < peaks.size(); which_entry++)
    {
        if(! grid.Get(peaks[which_entry], peak))
        {
            throw Exception("DiscreteVoidCluster::Swap(): cannot get grid data");
            return 0;
        }

        if(!peak.sample)
        {
            throw Exception("DiscreteVoidCluster::Swap(): empty peak");
            return 0;
        }

        if(! grid.Get(troughs[which_entry], trough))
        {
            throw Exception("DiscreteVoidCluster::Swap(): cannot get grid data");
            return 0;
        }

        if(trough.sample)
        {
            throw Exception("DiscreteVoidCluster::Swap(): non-empty trough");
            return 0;
        }

        if(peak.sample->position.Dimension() != troughs[which_entry].size())
        {
            throw Exception("DiscreteVoidCluster::Swap(): incompatible dimension");
            return 0;
        }

        trough.sample = peak.sample; peak.sample = 0;
        for(int i = 0; i < trough.sample->position.Dimension(); i++)
        {
            const_cast<Sample *>(trough.sample)->position[i] = troughs[which_entry][i];
        }

        if(! grid.Put(peaks[which_entry], peak))
        {
            throw Exception("DiscreteVoidCluster::Swap(): cannot put grid data");
            return 0;
        }

        if(! grid.Put(troughs[which_entry], trough))
        {
            throw Exception("DiscreteVoidCluster::Swap(): cannot put grid data");
            return 0;
        }
    }

    // done
    return 1;
}

double DiscreteVoidCluster::Mean(const Array<Datum> & grid)
{
    double mean = 0;
    
    vector<Datum> data;
    grid.Get(data);

    for(unsigned int i = 0; i < data.size(); i++)
    {
        mean += data[i].value;
    }

    mean /= data.size();

    return mean;
}

double DiscreteVoidCluster::Variance(const Array<Datum> & grid)
{
    const double mean = Mean(grid);

    vector<Datum> data;
    grid.Get(data);

    double variance = 0;
    for(unsigned int i = 0; i < data.size(); i++)
    {
        double diff = data[i].value - mean;
        variance += diff*diff;
    }

    variance /= data.size();

    return variance;
}

double DiscreteVoidCluster::Energy(const Array<Datum> & grid)
{
    return Variance(grid);
}

int DiscreteVoidCluster::Value(Array<double> & value) const
{
    return VoidCluster::Value(_grid, value);
}

int DiscreteVoidCluster::Find(const int peak_class_id, const int swap_size, vector<Sample::Position> & peaks_out, vector<Sample::Position> & troughs_out) const
{
    vector< vector<int> > peaks, troughs;
    if(! Find(_grid, peak_class_id, swap_size, peaks, troughs))
    {
        return 0;
    }
    
    peaks_out = vector<Sample::Position>(peaks.size());
    for(unsigned int i = 0; i < peaks_out.size(); i++)
    {
        Convert(Convert(peaks[i]), peaks_out[i]);
    }

    troughs_out = vector<Sample::Position>(troughs.size());
    for(unsigned int i = 0; i < troughs_out.size(); i++)
    {
        Convert(Convert(troughs[i]), troughs_out[i]);
    }

    return 1;
}
