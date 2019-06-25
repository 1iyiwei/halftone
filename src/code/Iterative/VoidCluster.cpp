/*
  VoidCluster.cpp

  Li-Yi Wei
  05/03/2009

*/

#include <algorithm>
using namespace std;

#include <math.h>

#include "VoidCluster.hpp"
#include "SequentialCounter.hpp"
#include "OnionLayerCounter.hpp"
#include "RandomCounter.hpp"
#include "Math.hpp"
#include "Exception.hpp"

VoidCluster::VoidCluster(const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size) : _class_weight(class_weight), _relative_splat_size(relative_splat_size), _init_swap_size(init_swap_size), _swap_size(init_swap_size)
{
    // nothing else to do
}

VoidCluster::~VoidCluster(void)
{
    // nothing else to do
}

VoidCluster::RunStatus VoidCluster::Init(const vector<Sample> & input)
{
    _swap_size = _init_swap_size;

    _samples = input;

    if(! NumSamples(_samples, _num_samples))
    {
        return FAILURE;
    }

    return OK;
}

VoidCluster::RunStatus VoidCluster::Next(vector<Sample> & output)
{
    throw Exception("VoidCluster::Next() should not be called");
    return FAILURE;
}

int VoidCluster::Value(Array<double> & value) const
{
    throw Exception("VoidCluster::Value() shouldn't be called");
    return 0;
}

int VoidCluster::Find(const int peak_class_id, const int swap_size, vector<Sample::Position> & peaks, vector<Sample::Position> & troughs) const
{
    throw Exception("VoidCluster::Find() shouldn't be called");
    return 0;
}

vector<double> VoidCluster::Convert(const vector<int> & input)
{
    vector<double> output(input.size());
    
    for(unsigned int i = 0; i < output.size(); i++)
    {
        output[i] = input[i];
    }

    return output;
}

vector<int> VoidCluster::Convert(const vector<double> & input)
{
    vector<int> output(input.size());
    
    for(unsigned int i = 0; i < output.size(); i++)
    {
        output[i] = static_cast<int>(input[i]);
    }

    return output;
}

void VoidCluster::Convert(const vector<double> & input, Sample::Position & output)
{
    output = Sample::Position(input.size());

    for(int i = 0; i < output.Dimension(); i++)
    {
        output[i] = input[i];
    }
}

float VoidCluster::SplatSize(const vector<int> & grid_size, const int num_samples)
{
    return SplatSize(Convert(grid_size), num_samples);
}

float VoidCluster::SplatSize(const vector<double> & domain_size, const int num_samples)
{
    double total = 1;
    for(unsigned int i = 0; i < domain_size.size(); i++)
    {
        total *= domain_size[i];
    }

#if 1
    // hypersphere
    const float radius = Math::HyperSphereRadius(domain_size.size(), total*1.0/num_samples);
#else
    // hypercube
    const float radius = max(1.0, pow(total*1.0/num_samples, 1.0/domain_size.size())*0.5*sqrt(domain_size.size()*1.0));
#endif

    return radius;
}

float VoidCluster::SplatSize(const vector<int> & grid_size, const vector<int> & num_samples)
{
    return SplatSize(Convert(grid_size), num_samples);
}

float VoidCluster::SplatSize(const vector<double> & domain_size, const vector<int> & num_samples)
{
    int total_count = 0;
    for(unsigned int i = 0; i < num_samples.size(); i++)
    {
        total_count += num_samples[i];
    }

    return SplatSize(domain_size, total_count);
}

int VoidCluster::Scatter(const vector<Sample> & samples, const vector<float> & weight, const float splat_size, Array<Datum> & grid)
{
    const int gaussian_extent = 3; // std multiple for splat extend

    const float std = splat_size/2; // 95% values fall into 2 std of Gaussian
    const float variance = std*std;
    const float filter_radius = std*gaussian_extent;

    Datum data;

    OnionLayerCounter counter(grid.Dimension(), filter_radius*filter_radius);

    vector<int> grid_size; grid.Size(grid_size);
    vector<int> index(grid.Dimension());
    vector<int> center(grid.Dimension());

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
            if(center.size() != sample.position.Dimension())
            {
                throw Exception("DiscreteVoidCluster::Scatter(): weird");
                return 0;
            }

            for(unsigned int i = 0; i < center.size(); i++)
            {
                center[i] = static_cast<int>(floor(sample.position[i]+0.5));
            }

            counter.Reset();

            do
            {
                counter.Get(index);

                double radius2 = 0;
                for(unsigned int i = 0; i < index.size(); i++)
                {
                    const double center_shift = sample.position[i]-center[i];
                    const double adjusted_value = index[i] - center_shift;
                    radius2 += adjusted_value*adjusted_value;
                }

                const float gaussian_value = exp(-radius2/variance);

                // toroidal boundary
                for(unsigned int i = 0; i < index.size(); i++)
                {
                    index[i] = index[i] + center[i];

                    index[i] = ((index[i]%grid_size[i])+grid_size[i])%grid_size[i];
                }

                if(! grid.Get(index, data))
                {       
                    throw Exception("DiscreteVoidCluster::Scatter(): cannot get data");
                    return 0;
                }

                data.value += gaussian_value*sample.value*weight[sample.id];

                if(! grid.Put(index, data))
                {       
                    throw Exception("DiscreteVoidCluster::Scatter(): cannot put data");
                    return 0;
                }
            }
            while(counter.Next());
        }
    }

    // done
    return 1;
}

int VoidCluster::Scatter(const vector<Sample> & samples, const ClassWeight & class_weight, const int which_class, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first, Array<Datum> & grid)
{
    // clear values, but not sample links
    if(clear_first)
    {
        vector<Datum> data;
        grid.Get(data);
        for(unsigned int i = 0; i < data.size(); i++)
        {
            data[i].value = 0;
        }
        grid.Put(data);
    }

    const int num_classes = class_selection.size();

    vector<int> scatter_classes(num_classes);
    vector<float> weight(num_classes);
    vector<int> num_samples(num_classes);
    if(! NumSamples(samples, num_samples))
    {
        return 0;
    }

    vector<int> grid_size; grid.Size(grid_size);

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
            throw Exception("DiscreteVoidCluster::Scatter(): cannot get class weights");
            return 0;
        }

        if(weight.size() != class_selection.size())
        {
            throw Exception("DiscreteVoidCluster::Scatter(): dimension mismatch");
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

        const float splat_size = SplatSize(grid_size, local_num_samples)*relative_splat_size;

        if(splat_size <= 0)
        {
            throw Exception("DiscreteVoidCluster::Scatter(): cannot compute splat sizes");
            return 0;
        }

        if(! Scatter(samples, weight, splat_size, grid))
        {
            throw Exception("DiscreteVoidCluster::Scatter(): error in scatter samples");
            return 0;
        }
    }
    while(counter.Next());

    // done
    return 1;
}

int VoidCluster::NumSamples(const vector<Sample> & samples, vector<int> & num_samples)
{
    int max_class_id = -1;

    for(unsigned int i = 0; i < samples.size(); i++)
    {
        if(samples[i].id > max_class_id)
        {
            max_class_id = samples[i].id;
        }
    }

    const int num_classes = max_class_id + 1;
    num_samples = vector<int>(num_classes, 0);

    for(unsigned int i = 0; i < samples.size(); i++)
    {
        if((samples[i].id < 0) || (samples[i].id >= num_samples.size()))
        {
            return 0;
        }
        else
        {
            num_samples[samples[i].id]++;
        }
    }

    // done
    return 1;
}

int VoidCluster::FindPeaks(const vector<IndexDatum> & input, const int peak_class_id, const int swap_size, vector<IndexDatum> & peaks)
{
    peaks.clear();

    for(unsigned int which_input = 0; which_input < input.size(); which_input++)
    {
        const IndexDatum & holder = input[which_input];
        const Datum & data = holder.data;

        if(data.sample && (data.sample->id == peak_class_id))
        {
            // potential peak
            int location = -1;
            for(unsigned int i = 0; i < peaks.size(); i++)
            {
                if(peaks[i].data.value < data.value)
                {
                    location = i; break;
                }
            }

            // insert before
            peaks.insert(location >= 0 ? peaks.begin()+location : peaks.end(), holder);

            if(peaks.size() > swap_size) peaks.pop_back();
        }
    }

    return peaks.size();
}

int VoidCluster::FindTroughs(const vector<IndexDatum> & input, const int swap_size, vector<IndexDatum> & troughs)
{
    troughs.clear();

    for(unsigned int which_input = 0; which_input < input.size(); which_input++)
    {
        const IndexDatum & holder = input[which_input];
        const Datum & data = holder.data;

        if(!data.sample)
        {
            // potential trough
            int location = -1;
            for(unsigned int i = 0; i < troughs.size(); i++)
            {
                if(troughs[i].data.value > data.value)
                {
                    location = i; break;
                }
            }

            // insert before
            troughs.insert(location >= 0 ? troughs.begin()+location : troughs.end(), holder);

            if(troughs.size() > swap_size) troughs.pop_back();
        }
    }

    return troughs.size();
}

int VoidCluster::FindPeaks(const Array<Datum> & grid, const int peak_class_id, const int swap_size, vector< vector<int> > & peaks_out)
{
    vector<IndexDatum> peaks;
    
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

        if((index_datum.data.sample) && (index_datum.data.sample->id == peak_class_id))
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
            // trough or wrong sample id
            // so do nothing
        }
    }
    while(counter.Next());

    peaks_out = vector< vector<int> >(peaks.size());

    for(unsigned int i = 0; i < peaks_out.size(); i++)
    {
        peaks_out[i] = peaks[i].index;
    } 

    // done
    return peaks_out.size();
}
    
int VoidCluster::FindTroughs(const Array<Datum> & grid, const int swap_size, vector< vector<int> > & troughs_out)
{
    vector<IndexDatum> troughs;
    
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
        else
        {
            // not trough
            // so do nothing
        }
    }
    while(counter.Next());

    troughs_out = vector< vector<int> >(troughs.size());

    for(unsigned int i = 0; i < troughs_out.size(); i++)
    {
        troughs_out[i] = troughs[i].index;
    }

    // done
    return troughs_out.size();
}

int VoidCluster::Value(const Array<Datum> & input, Array<double> & output)
{
    vector<int> input_size; input.Size(input_size);

    output = Array<double>(input_size);

    vector<Datum> input_data; input.Get(input_data);
    vector<double> output_data(input_data.size());

    for(unsigned int i = 0; i < output_data.size(); i++)
    {
        output_data[i] = input_data[i].value;
    }

    output.Put(output_data);

    return 1;
}
