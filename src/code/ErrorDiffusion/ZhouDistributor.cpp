/*
  ZhouDistributor.cpp

  Li-Yi Wei
  03/02/2010

*/

#include "ZhouDistributor.hpp"
#include "Exception.hpp"

ZhouDistributor::ZhouDistributor(const Pixel & max_value): VarCoeffDistributor(max_value)
{
    // replace the coefficient table
    for(int i = 0; i < _MODIFIED_COEFFS_TABLE_SIZE; i++)
    {
        const int index = _MODIFIED_COEFFS_TABLE[i].index;
        const int mirror_index = _var_coeffs_table.size()-index-1;

        if((index < 0) || (index >= _var_coeffs_table.size())) throw Exception("weird");
        if((mirror_index < 0) || (mirror_index >= _var_coeffs_table.size())) throw Exception("weird");
        
        _var_coeffs_table[index] = _var_coeffs_table[mirror_index] = _MODIFIED_COEFFS_TABLE[i].coeffs;
        _var_coeffs_table[index].sum = _var_coeffs_table[mirror_index].sum = (_var_coeffs_table[index].r + _var_coeffs_table[index].dl  + _var_coeffs_table[index].d);
    }
}

ZhouDistributor::~ZhouDistributor(void)
{
    // nothing else to do
}

const int ZhouDistributor::_MODIFIED_COEFFS_TABLE_SIZE = 18;
// the sum coefficient is not used
const ZhouDistributor::IndexedThreeCoeffsInt ZhouDistributor::_MODIFIED_COEFFS_TABLE[_MODIFIED_COEFFS_TABLE_SIZE] =
{
    0, {13, 0, 5, 0},
    1, {1300249, 0, 499250, 0},
    2, {214114, 287, 99357, 0},
    3, {351854, 0, 199965, 0},
    4, {801100, 0, 490999, 0},
    10, {704075, 297466, 303694, 0},
    22, {46613, 31917, 21469, 0},
    32, {47482, 30617, 21900, 0},
    44, {43024, 42131, 14826, 0},
    64, {36411, 43219, 20369, 0},
    72, {38477, 53843, 7678, 0},
    77, {40503, 51547, 7948, 0},
    85, {35865, 34108, 30026, 0},
    95, {34117, 36899, 28983, 0},
    102, {35464, 35049, 29485, 0},
    107, {16477, 18810, 14712, 0},
    112, {33360, 37954, 28685, 0},
    127, {35269, 36066, 28664, 0}
};  
