/*
  ZhouThresholder.hpp

  BingFeng Zhou's SIGGRAPH 2003 algorithm

  Li-Yi Wei
  03/01/2010

*/

#ifndef _ZHOU_THRESHOLDER_HPP
#define _ZHOU_THRESHOLDER_HPP

#include "Thresholder.hpp"

class ZhouThresholder : public Thresholder
{
public:
    ZhouThresholder(const vector<Pixel> & palette, const Pixel & max_value);
    virtual ~ZhouThresholder(void);

protected:
    virtual int Nearest(const Pixel & query, const vector<Pixel> & palette) const;

protected:
    int GetModulation(const Pixel & input, const Pixel & max_value, vector<float> & modulation) const; 
    int GetModulation(const PixelType & input, const PixelType & max_value, float & modulation) const;

protected:
    const Pixel _max_value;

    struct Modulation
    {
        int key_level;
        float strength;
    };

    vector<Modulation> _modulation_table;

    mutable vector<float> _modulation;

private: 
    static const int _VAR_COEFFS_TABLE_SIZE;
    static const int _MODULATION_TABLE_SIZE;
    static const Modulation _MODULATION_TABLE[];
};


#endif
