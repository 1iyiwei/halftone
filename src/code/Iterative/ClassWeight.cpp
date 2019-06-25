/*
  ClassWeight.cpp

  Li-Yi Wei
  05/04/2009

*/

#include "ClassWeight.hpp"
#include "Exception.hpp"

ClassWeight::~ClassWeight(void)
{
    // nothing to do
}

int ClassWeight::Get(const int main_class, const vector<int> & source_classes, vector<float> & weights) const
{
    throw Exception("ClassWeight::Weight() should not be called");
    return 0;
}
