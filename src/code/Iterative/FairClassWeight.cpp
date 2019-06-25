/*
  FairClassWeight.cpp

  Li-Yi Wei
  05/05/2009

*/

#include "FairClassWeight.hpp"

FairClassWeight::FairClassWeight(const vector<int> & num_samples_per_class) : _num_samples(num_samples_per_class)
{
    // nothing to do
}

FairClassWeight::~FairClassWeight(void)
{
    // nothing to do
}

int FairClassWeight::Get(const int main_class, const vector<int> & source_classes, vector<float> & weights) const
{
    if(source_classes.size() != _num_samples.size())
    {
        return 0;
    }

    int all_sum = 0;
    int partial_sum = 0;

    for(unsigned int i = 0; i < source_classes.size(); i++)
    {
        all_sum += _num_samples[i];
        partial_sum += (source_classes[i] > 0)*_num_samples[i];
    }

    const float weight = (partial_sum > 0 ? all_sum*1.0/partial_sum : 0);

    weights = vector<float>(source_classes.size(), weight);

    for(unsigned int i = 0; i < weights.size(); i++)
    {
        weights[i] *= (source_classes[i] > 0);
    }

    // done
    return 1;
}
