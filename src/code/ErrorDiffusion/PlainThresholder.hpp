/*
  PlainThresholder.hpp
  
  the default/plain/vanilla thresholder class

  Li-Yi Wei
  03/01/2010

*/

#ifndef _PLAIN_THRESHOLDER_HPP
#define _PLAIN_THRESHOLDER_HPP

#include "Thresholder.hpp"

class PlainThresholder : public Thresholder
{
public:
    PlainThresholder(const vector<Pixel> & palette);
    virtual ~PlainThresholder(void);
};

#endif
