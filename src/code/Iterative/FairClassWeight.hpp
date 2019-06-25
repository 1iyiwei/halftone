/*
  FairClassWeight.hpp

  weigh class samples so that each splat collection has same impact

  Li-Yi Wei
  05/05/2009

*/

#ifndef _FAIR_CLASS_WEIGHT_HPP
#define _FAIR_CLASS_WEIGHT_HPP

#include "ClassWeight.hpp"

class FairClassWeight : public ClassWeight
{
public:
    FairClassWeight(const vector<int> & num_samples_per_class);
    ~FairClassWeight(void);

    int Get(const int main_class, const vector<int> & source_classes, vector<float> & weights) const;

protected:
    const vector<int> _num_samples;
};
#endif
