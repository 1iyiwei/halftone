/*
  ZhouThresholder.cpp

  Li-Yi Wei
  03/01/2010

*/

#include "ZhouThresholder.hpp"

#include "Exception.hpp"
#include "Utility.hpp"
#include "Random.hpp"

ZhouThresholder::ZhouThresholder(const vector<Pixel> & palette, const Pixel & max_value): Thresholder(palette), _max_value(max_value), _modulation_table(2*_MODULATION_TABLE_SIZE), _modulation(max_value.Dimension())
{
    // build the modulation table
    for(int i = 0; i < _MODULATION_TABLE_SIZE; i++)
    {
        _modulation_table[i] = _modulation_table[2*_MODULATION_TABLE_SIZE-i-1] = _MODULATION_TABLE[i];
        _modulation_table[2*_MODULATION_TABLE_SIZE-i-1].key_level = /*_var_coeffs_table.size()*/_VAR_COEFFS_TABLE_SIZE - _modulation_table[i].key_level - 1;
    }
}

ZhouThresholder::~ZhouThresholder(void)
{
    // nothing else to do
}

int ZhouThresholder::Nearest(const Pixel & query, const vector<Pixel> & palette) const
{
    if(!GetModulation(query, _max_value, _modulation)) return 0;

    Pixel modified_query(query);

    for(int i = 0; i < modified_query.Dimension(); i++)
    {
        modified_query[i] -= _modulation[i]*Random::UniformRandom()*(_max_value[i]+1)*0.5;
    }

    return Thresholder::Nearest(modified_query, palette);
}

int ZhouThresholder::GetModulation(const Pixel & input, const Pixel & max_value, vector<float> & modulation) const
{
    if(input.Dimension() != max_value.Dimension()) return 0;

    if(modulation.size() != input.Dimension()) modulation = vector<float>(input.Dimension());

    for(unsigned int i = 0; i < modulation.size(); i++)
    {
        if(! GetModulation(input[i], max_value[i], modulation[i])) return 0;
    }

    return 1;
}

int ZhouThresholder::GetModulation(const PixelType & input, const PixelType & max_value, float & modulation) const
{
    const int table_size = /*_var_coeffs_table.size()*/_VAR_COEFFS_TABLE_SIZE;

    const double entry = min((table_size-1)*1.0, max(0.0, (table_size-1)*input*1.0/max_value));

    int entry_lo = 0;
    int entry_hi = -1;

    // linear search
    for(unsigned int i = 0; i < _modulation_table.size(); i++)
    {
        if(_modulation_table[i].key_level <= entry) entry_lo = i;
    }
    for(int i = static_cast<int>(_modulation_table.size())-1; i >= 0; i--)
    {
        if(entry <= _modulation_table[i].key_level) entry_hi = i;
    }

    if(entry_hi < entry_lo)
    {
        throw Exception("ZhouThresholder::GetModulation() : entry_hi < entry_lo");
    }

    const float modulation_lo = _modulation_table[entry_lo].strength;
    const float modulation_hi = _modulation_table[entry_hi].strength;
    
    if(entry_lo == entry_hi)
    {
        modulation = modulation_lo;
    }
    else
    {
        const double weight_hi = entry - entry_lo;
        const double weight_lo = 1.0 - weight_hi;

        modulation = modulation_lo*weight_lo + modulation_hi*weight_hi;
    }

    // done
    return 1;
}

const int ZhouThresholder::_VAR_COEFFS_TABLE_SIZE = 256;
const int ZhouThresholder::_MODULATION_TABLE_SIZE = 9;
const ZhouThresholder::Modulation ZhouThresholder::_MODULATION_TABLE[_MODULATION_TABLE_SIZE] =
{
    0, 0.0,
    44, 0.34,
    64, 0.50,
    85, 1.00,
    95, 0.71,
    102, 0.50,
    107, 0.70,
    112, 0.79,
    127, 1.00
};
