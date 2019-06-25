/*
  RainbowClassWeight.hpp

  weigh class samples to reach equivalence to multi-class PD
  i.e. only worry about classes with <= number of samples

  Li-Yi Wei
  05/07/2009

*/

#ifndef _RAINBOW_CLASS_WEIGHT_HPP
#define _RAINBOW_CLASS_WEIGHT_HPP

#include "ClassWeight.hpp"
#include "FairClassWeight.hpp"

class RainbowClassWeight : public ClassWeight
{
public:
    RainbowClassWeight(const vector<int> & num_samples_per_class);
    ~RainbowClassWeight(void);

    int Get(const int main_class, const vector<int> & source_classes, vector<float> & weights) const;

protected:
    const vector<int> _num_samples;
    const FairClassWeight _fair_class_weight;
};
#endif
