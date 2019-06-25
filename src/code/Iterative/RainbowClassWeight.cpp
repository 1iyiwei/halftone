/*
  RainbowClassWeight.cpp

  Li-Yi Wei
  05/07/2009

*/

#include "RainbowClassWeight.hpp"

RainbowClassWeight::RainbowClassWeight(const vector<int> & num_samples_per_class) : _num_samples(num_samples_per_class), _fair_class_weight(num_samples_per_class)
{
    // nothing to do
}

RainbowClassWeight::~RainbowClassWeight(void)
{
    // nothing to do
}

int RainbowClassWeight::Get(const int main_class, const vector<int> & source_classes_input, vector<float> & weights) const
{
    vector<int> source_classes(source_classes_input);
    
    if(source_classes.size() != _num_samples.size())
    {
        return 0;
    }

    if((main_class < 0) || (main_class >= source_classes.size()))
    {
        // error
        return 0;
    }
    else
    {
        for(unsigned int i = 0; i < source_classes.size(); i++)
        {
            if(_num_samples[i] > _num_samples[main_class])
            {
                source_classes[i] = 0;
            }
        }
    }

    return _fair_class_weight.Get(main_class, source_classes, weights);
}
