/*
  VarCoeffDistributor.hpp

  Li-Yi Wei
  03/02/2010

*/

#ifndef _VAR_COEFF_DISTRIBUTOR_HPP
#define _VAR_COEFF_DISTRIBUTOR_HPP

#include "Distributor.hpp"

class VarCoeffDistributor : public Distributor
{
public:
    VarCoeffDistributor(const Pixel & max_value);
    virtual ~VarCoeffDistributor(void);

// protected:
    int Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const;

protected:
    struct ThreeCoeffsFloat
    {
        float r;        /* right */
        float dl;       /* down-left */
        float d;        /* down */
    };

    int GetCoeffs(const Pixel & input, const Pixel & max_value, vector<ThreeCoeffsFloat> & coefficients) const; 
    int GetCoeffs(const PixelType & input, const PixelType & max_value, ThreeCoeffsFloat & coefficient) const;

protected:
    const Pixel _max_value;
    
    mutable vector<ThreeCoeffsFloat> _coefficients;
    mutable vector<float> _offset_coefficients;

    struct ThreeCoeffsInt
    {
        int r;        /* right */
        int dl;       /* down-left */
        int d;        /* down */
        int sum;      /* sum */
    };

    static void Convert(const ThreeCoeffsInt & input, ThreeCoeffsFloat & output);

    vector<ThreeCoeffsInt> _var_coeffs_table;

private:
    static const int _VAR_COEFFS_TABLE_SIZE;
    static const ThreeCoeffsInt _VAR_COEFFS_TABLE[];
};

#endif
