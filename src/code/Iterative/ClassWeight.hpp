/*
  ClassWeight.hpp

  base class
  specifying range weights for different classes under different combinations

  Li-Yi Wei
  05/04/2009

*/

#ifndef _CLASS_WEIGHT_HPP
#define _CLASS_WEIGHT_HPP

#include <vector>
using namespace std;

class ClassWeight
{
public:
    virtual ~ClassWeight(void) = 0;

    // return the weights of source_classes
    // source_classes is a binary array indicating 
    // the presence/absence of individual classes
    // main_class is the one under consideration for moving
    // return 1 if ok, 0 if error
    virtual int Get(const int main_class, const vector<int> & source_classes, vector<float> & weights) const;
};
#endif
