/*
  ContinuousVoidCluster.cpp

  Li-Yi Wei
  05/10/2009

*/

#include <iostream>
#include <algorithm>
using namespace std;

#include <math.h>

#include "ContinuousVoidCluster.hpp"
#include "SequentialCounter.hpp"
#include "Exception.hpp"

//#define EFFICIENT_SCATTERING

ContinuousVoidCluster::ContinuousVoidCluster(const vector<double> & domain_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size) : VoidCluster(class_weight, relative_splat_size, init_swap_size), _domain_size(domain_size)
{
    // nothing else to do
}

ContinuousVoidCluster::~ContinuousVoidCluster(void)
{
    // nothing else to do
}

ContinuousVoidCluster::RunStatus ContinuousVoidCluster::Init(const vector<Sample> & input)
{
    RunStatus status = FAILURE;

    if((status = VoidCluster::Init(input)) != OK)
    {
        return status;
    }

    _bag = vector<Datum>(_samples.size());
    for(unsigned int i = 0; i < _bag.size(); i++)
    {
        _bag[i].value = 0;
        _bag[i].sample = &_samples[i];
    }

    return OK;
}

ContinuousVoidCluster::RunStatus ContinuousVoidCluster::Next(vector<Sample> & output)
{
    const int num_classes = _num_samples.size();

    vector<int> class_selection(num_classes);

    for(int which_class = 0; which_class < num_classes; which_class++)
    {
        // scatter
        class_selection = vector<int>(num_classes, 1);
        if(! Scatter(_domain_size, _samples, _class_weight, which_class, class_selection, _relative_splat_size, 1))
        {
            throw Exception("ContinuousVoidCluster::Next(): cannot scatter initial samples");
            return FAILURE;
        }

        // find replacements
        vector<Datum> peaks;
        vector<Sample::Position> troughs;

        if(! Find(_bag, which_class, _swap_size, peaks, troughs))
        {
            throw Exception("ContinuousVoidCluster::Next(): error in finding peaks and troughs");
            return FAILURE;
        }
        
        // energy measurement
        const double energy_old = Energy(_bag);

#ifdef EFFICIENT_SCATTERING
        // scatter, remove value from which_class
        class_selection = vector<int>(num_classes, 0);
        class_selection[which_class] = -1;
        if(! Scatter(_domain_size, _samples, _class_weight, which_class, class_selection, _relative_splat_size, 0))
        {
            throw Exception("ContinuousVoidCluster::Next(): cannot scatter-remove target_class samples");
            return FAILURE;
        }
#endif

        // perform replacement
        if(! Swap(peaks, troughs))
        {
            throw Exception("ContinuousVoidCluster::Swap(): error in swapping peaks and troughs");
            return FAILURE;
        }
        
#ifdef EFFICIENT_SCATTERING
        // scatter, add value from which_class
        class_selection = vector<int>(num_classes, 0);
        class_selection[which_class] = 1;
        if(! Scatter(_domain_size, _samples, _class_weight, which_class, class_selection, _relative_splat_size, 0))
        {
            throw Exception("ContinuousVoidCluster::Next(): cannot scatter-add target_class samples");
            return FAILURE;
        }
#else
        class_selection = vector<int>(num_classes, 1);
        if(! Scatter(_domain_size, _samples, _class_weight, which_class, class_selection, _relative_splat_size, 1))
        {
            throw Exception("ContinuousVoidCluster::Next(): cannot scatter initial samples");
            return FAILURE;
        } 
#endif
        // gather samples
        output = _samples;

        // energy measurement and update swap size if necessary
        const double energy_new = Energy(_bag);

        // cerr << "energy old and new: " << energy_old << " " << energy_new << endl;
        if(energy_new >= energy_old)
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

int ContinuousVoidCluster::Scatter(const vector<double> & domain_size, const vector<Sample> & samples, const vector<float> & weight, const float splat_size, vector<Datum> & output)
{
    const float std = splat_size/2; // 95% values fall into 2 std of Gaussian
    const float variance = std*std;

    for(unsigned int which_sample = 0; which_sample < samples.size(); which_sample++)
    {
        const Sample & sample = samples[which_sample];

        if((sample.id < 0) || (sample.id >= weight.size()))
        {
            throw Exception("DiscreteVoidCluster::Scatter() : illegal sample id");
            return 0;
        }

        if(weight[sample.id] <= 0)
        {
            // do nothing
        }
        else
        {
            for(unsigned int which_receiver = 0; which_receiver < output.size(); which_receiver++)
            {
                Datum & data = output[which_receiver];

                if(!data.sample)
                {
                    throw Exception("ContinuousVoidCluster::Scatter(): no receiver sample");
                    return 0;
                }

                const Sample & target = *data.sample;

                if(sample.position.Dimension() != target.position.Dimension())
                {
                    throw Exception("ContinuousVoidCluster::Scatter(): incompatible sample dimensions");
                    return 0;
                }

                double radius2 = 0;
                for(unsigned int i = 0; i < sample.position.Dimension(); i++)
                {
                    double diff = fabs(sample.position[i]-target.position[i]);

                    // toroidal boundary
                    diff = min(diff, domain_size[i]-diff);

                    radius2 += diff*diff;
                }
                
                const float gaussian_value = exp(-radius2/variance);

                data.value += gaussian_value*sample.value*weight[sample.id];
            }
        }
    }

    // done
    return 1;
}

int ContinuousVoidCluster::Scatter(const vector<double> & domain_size, const vector<Sample> & samples, const ClassWeight & class_weight, const int which_class, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first, vector<Datum> & output)
{
    // clear values, but not sample links
    if(clear_first)
    {
        for(unsigned int i = 0; i < output.size(); i++)
        {
            output[i].value = 0;
        }
    }

    const int num_classes = class_selection.size();

    vector<int> scatter_classes(num_classes);
    vector<float> weight(num_classes);
    vector<int> num_samples(num_classes);
    if(! NumSamples(samples, num_samples))
    {
        return 0;
    }

    vector<int> counter_min(num_classes, 0);
    counter_min[which_class] = 1;
    vector<int> counter_max(num_classes, 1);
    
    SequentialCounter counter(num_classes, counter_min, counter_max);
    counter.Reset();
        
    do
    {
        counter.Get(scatter_classes);
        
        if(! class_weight.Get(which_class, scatter_classes, weight))
        {
            throw Exception("ContinuousVoidCluster::Scatter(): cannot get class weights");
            return 0;
        }

        if(weight.size() != class_selection.size())
        {
            throw Exception("ContinuousVoidCluster::Scatter(): dimension mismatch");
            return 0;
        }
        
        for(unsigned int i = 0; i < weight.size(); i++)
        {
            weight[i] *= class_selection[i];
        }

        vector<int> local_num_samples(num_samples);
        for(unsigned int i = 0; i < local_num_samples.size(); i++)
        {
            local_num_samples[i] *= scatter_classes[i];
        }

        const float splat_size = SplatSize(domain_size, local_num_samples)*relative_splat_size;

        if(splat_size <= 0)
        {
            throw Exception("ContinuousVoidCluster::Scatter(): cannot compute splat sizes");
            return 0;
        }

        if(! Scatter(domain_size, samples, weight, splat_size, output))
        {
            throw Exception("ContinuousVoidCluster::Scatter(): error in scatter samples");
            return 0;
        }
    }
    while(counter.Next());

    // done
    return 1;
}

int ContinuousVoidCluster::Scatter(const vector<double> & domain_size, const vector<Sample> & input, const ClassWeight & class_weight, const int main_class_id, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first)
{
    throw Exception("ContinuousVoidCluster::Scatter() should not be called");
    return 0;
}

int ContinuousVoidCluster::Find(const vector<Datum> & input, const int peak_class_id, const int swap_size, vector<Datum> & peaks, vector<Sample::Position> & troughs) const
{
    if(! FindPeaks(input, peak_class_id, swap_size, peaks))
    {
        return 0;
    } 

    if(! FindTroughs(peak_class_id, swap_size, troughs))
    {
        return 0;
    }

    // trim
    while(peaks.size() > troughs.size())
    {
        peaks.pop_back();
    }
    while(troughs.size() > peaks.size())
    {
        troughs.pop_back();
    }

    return peaks.size();
}

int ContinuousVoidCluster::FindPeaks(const vector<Datum> & input, const int peak_class_id, const int swap_size, vector<Datum> & peaks)
{
    peaks.clear();

    for(unsigned int which_input = 0; which_input < input.size(); which_input++)
    {
        const Datum data = input[which_input];
        
        if(data.sample && (data.sample->id == peak_class_id))
        {
            // potential peak
            int location = -1;
            for(unsigned int i = 0; i < peaks.size(); i++)
            {
                if(peaks[i].value < data.value)
                {
                    location = i; break;
                }
            }

            // insert before
            peaks.insert(location >= 0 ? peaks.begin()+location : peaks.end(), data);

            if(peaks.size() > swap_size) peaks.pop_back();
        }
    }

    return peaks.size();
}

int ContinuousVoidCluster::FindTroughs(const int peak_class_id, const int swap_size, vector<Sample::Position> & troughs) const
{
    throw Exception("ContinuousVoidCluster::FindTroughs() should not be called");
    return 0;
}

int ContinuousVoidCluster::Swap(const vector<Datum> & peaks, const vector<Sample::Position> & troughs)
{
    if(peaks.size() != troughs.size()) return 0;

    for(unsigned int i = 0; i < peaks.size(); i++)
    {
        if(!peaks[i].sample)
        {
            throw Exception("ContinuousVoidCluster::Swap(): null peak sample");
            return 0;
        }

        if(peaks[i].sample->position.Dimension() != troughs[i].Dimension())
        {
            throw Exception("ContinuousVoidCluster::Swap(): position dimension mismatch");
            return 0;
        }

        const_cast<Sample *>(peaks[i].sample)->position = troughs[i];
    }

    return 1;
}

double ContinuousVoidCluster::Energy(const vector<Datum> & bag)
{
    double energy = 0;

    for(unsigned int i = 0; i < bag.size(); i++)
    {
        energy += bag[i].value*bag[i].value;
    }

    if(bag.size() > 0)
    {
        energy /= bag.size();
    }

    return energy;
}

int ContinuousVoidCluster::Find(const int peak_class_id, const int swap_size, vector<Sample::Position> & peaks_out, vector<Sample::Position> & troughs_out) const
{
    vector<Datum> peaks;

    if(!Find(_bag, peak_class_id, swap_size, peaks, troughs_out))
    {
        return 0;
    }

    peaks_out = vector<Sample::Position>(peaks.size());

    for(unsigned int i = 0; i < peaks_out.size(); i++)
    {
        if(!peaks[i].sample)
        {
            return 0;
        }
        else
        {
            peaks_out[i] = peaks[i].sample->position;
        }
    }

    return 1;
}
