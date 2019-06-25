/*
  VarCoeffDistributor.cpp

  Li-Yi Wei
  03/02/2010

*/

#include <algorithm>
using namespace std;

#include <math.h>

#include "VarCoeffDistributor.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

VarCoeffDistributor::VarCoeffDistributor(const Pixel & max_value): _max_value(max_value), _coefficients(max_value.Dimension()), _offset_coefficients(max_value.Dimension()), _var_coeffs_table(_VAR_COEFFS_TABLE_SIZE)
{
    for(unsigned int i = 0; i < _var_coeffs_table.size(); i++)
    {
        _var_coeffs_table[i] = _VAR_COEFFS_TABLE[i];
    }
}

VarCoeffDistributor::~VarCoeffDistributor(void)
{
    // nothing else to do
}

int VarCoeffDistributor::Coeff(const CoordinatePixel & input, vector<CoordinatePixel> & coefficients) const
{
    if(! GetCoeffs(input.value, _max_value, _coefficients)) throw Exception("cannot get coefficients");

    if(_offset_coefficients.size() != _max_value.Dimension())
    {
        _offset_coefficients = vector<float>(_max_value.Dimension());
    }

    if(coefficients.size() != 3)
    {
        coefficients = vector<CoordinatePixel>(3);
    }

    for(unsigned int i = 0; i < coefficients.size(); i++)
    {
        CoordinatePixel & coefficient = coefficients[i];
        coefficient = input;

        if(coefficient.coordinate.Dimension() < 2) throw Exception("coefficient.coordinate.Dimension() < 2!");

        switch(i%coefficients.size())
        {
        case 0:
            coefficient.coordinate[0] = 1;
            coefficient.coordinate[1] = 0;
            for(int j = 0; j < coefficient.value.Dimension(); j++)
            {
                coefficient.value[j] = _coefficients[j].r;
            }
            break;

        case 1:
            coefficient.coordinate[0] = -1;
            coefficient.coordinate[1] = 1;
            for(int j = 0; j < coefficient.value.Dimension(); j++)
            {
                coefficient.value[j] = _coefficients[j].dl;
            }
            break;

        case 2:
            coefficient.coordinate[0] = 0;
            coefficient.coordinate[1] = 1;
            for(int j = 0; j < coefficient.value.Dimension(); j++)
            {
                coefficient.value[j] = _coefficients[j].d;
            }
            break;

        default:
            throw Exception("VarCoeffDistributor::Coeff(): unanticipated error offset");
            break;
        }
    }

    // done
    return 1;
}


int VarCoeffDistributor::GetCoeffs(const Pixel & input, const Pixel & max_value, vector<ThreeCoeffsFloat> & coefficients) const
{
    if(input.Dimension() != max_value.Dimension()) return 0;

    if(coefficients.size() != input.Dimension()) coefficients = vector<ThreeCoeffsFloat>(input.Dimension());

    for(unsigned int i = 0; i < coefficients.size(); i++)
    {
        if(! GetCoeffs(input[i], max_value[i], coefficients[i])) return 0;
    }

    return 1;
}

int VarCoeffDistributor::GetCoeffs(const PixelType & input, const PixelType & max_value, ThreeCoeffsFloat & coefficient) const
{
    const int table_size = _var_coeffs_table.size();

    const double entry = min((table_size-1)*1.0, max(0.0, (table_size-1)*input*1.0/max_value));

    // table lookup + linear interpolation
    const int entry_lo = static_cast<int>(floor(entry));
    const int entry_hi = static_cast<int>(ceil(entry));

    ThreeCoeffsFloat coeffs_lo;
    ThreeCoeffsFloat coeffs_hi;
    Convert(_var_coeffs_table[entry_lo], coeffs_lo);
    Convert(_var_coeffs_table[entry_hi], coeffs_hi);

    if(entry_lo == entry_hi)
    {
        coefficient = coeffs_lo;
    }
    else
    {
        const double weight_hi = entry - entry_lo;
        const double weight_lo = 1.0 - weight_hi;

        coefficient.r = coeffs_lo.r*weight_lo + coeffs_hi.r*weight_hi;
        coefficient.dl = coeffs_lo.dl*weight_lo + coeffs_hi.dl*weight_hi;
        coefficient.d = coeffs_lo.d*weight_lo + coeffs_hi.d*weight_hi;
    }

    // done
    return 1;
}

void VarCoeffDistributor::Convert(const ThreeCoeffsInt & input, ThreeCoeffsFloat & output)
{
    output.r = input.r*1.0/input.sum;
    output.dl = input.dl*1.0/input.sum;
    output.d = input.d*1.0/input.sum;
}

const int VarCoeffDistributor::_VAR_COEFFS_TABLE_SIZE = 256;
const VarCoeffDistributor::ThreeCoeffsInt VarCoeffDistributor::_VAR_COEFFS_TABLE[VarCoeffDistributor::_VAR_COEFFS_TABLE_SIZE] =
{
    13,     0,     5,    18,     /*    0 */
    13,     0,     5,    18,     /*    1 */
    21,     0,    10,    31,     /*    2 */
     7,     0,     4,    11,     /*    3 */
     8,     0,     5,    13,     /*    4 */
    47,     3,    28,    78,     /*    5 */
    23,     3,    13,    39,     /*    6 */
    15,     3,     8,    26,     /*    7 */
    22,     6,    11,    39,     /*    8 */
    43,    15,    20,    78,     /*    9 */
     7,     3,     3,    13,     /*   10 */
   501,   224,   211,   936,     /*   11 */
   249,   116,   103,   468,     /*   12 */
   165,    80,    67,   312,     /*   13 */
   123,    62,    49,   234,     /*   14 */
   489,   256,   191,   936,     /*   15 */
    81,    44,    31,   156,     /*   16 */
   483,   272,   181,   936,     /*   17 */
    60,    35,    22,   117,     /*   18 */
    53,    32,    19,   104,     /*   19 */
   237,   148,    83,   468,     /*   20 */
   471,   304,   161,   936,     /*   21 */
     3,     2,     1,     6,     /*   22 */
   459,   304,   161,   924,     /*   23 */
    38,    25,    14,    77,     /*   24 */
   453,   296,   175,   924,     /*   25 */
   225,   146,    91,   462,     /*   26 */
   149,    96,    63,   308,     /*   27 */
   111,    71,    49,   231,     /*   28 */
    63,    40,    29,   132,     /*   29 */
    73,    46,    35,   154,     /*   30 */
   435,   272,   217,   924,     /*   31 */
   108,    67,    56,   231,     /*   32 */
    13,     8,     7,    28,     /*   33 */
   213,   130,   119,   462,     /*   34 */
   423,   256,   245,   924,     /*   35 */
     5,     3,     3,    11,     /*   36 */
   281,   173,   162,   616,     /*   37 */
   141,    89,    78,   308,     /*   38 */
   283,   183,   150,   616,     /*   39 */
    71,    47,    36,   154,     /*   40 */
   285,   193,   138,   616,     /*   41 */
    13,     9,     6,    28,     /*   42 */
    41,    29,    18,    88,     /*   43 */
    36,    26,    15,    77,     /*   44 */
   289,   213,   114,   616,     /*   45 */
   145,   109,    54,   308,     /*   46 */
   291,   223,   102,   616,     /*   47 */
    73,    57,    24,   154,     /*   48 */
   293,   233,    90,   616,     /*   49 */
    21,    17,     6,    44,     /*   50 */
   295,   243,    78,   616,     /*   51 */
    37,    31,     9,    77,     /*   52 */
    27,    23,     6,    56,     /*   53 */
   149,   129,    30,   308,     /*   54 */
   299,   263,    54,   616,     /*   55 */
    75,    67,    12,   154,     /*   56 */
    43,    39,     6,    88,     /*   57 */
   151,   139,    18,   308,     /*   58 */
   303,   283,    30,   616,     /*   59 */
    38,    36,     3,    77,     /*   60 */
   305,   293,    18,   616,     /*   61 */
   153,   149,     6,   308,     /*   62 */
   307,   303,     6,   616,     /*   63 */
     1,     1,     0,     2,     /*   64 */
   101,   105,     2,   208,     /*   65 */
    49,    53,     2,   104,     /*   66 */
    95,   107,     6,   208,     /*   67 */
    23,    27,     2,    52,     /*   68 */
    89,   109,    10,   208,     /*   69 */
    43,    55,     6,   104,     /*   70 */
    83,   111,    14,   208,     /*   71 */
     5,     7,     1,    13,     /*   72 */
   172,   181,    37,   390,     /*   73 */
    97,    76,    22,   195,     /*   74 */
    72,    41,    17,   130,     /*   75 */
   119,    47,    29,   195,     /*   76 */
     4,     1,     1,     6,     /*   77 */
     4,     1,     1,     6,     /*   78 */
     4,     1,     1,     6,     /*   79 */
     4,     1,     1,     6,     /*   80 */
     4,     1,     1,     6,     /*   81 */
     4,     1,     1,     6,     /*   82 */
     4,     1,     1,     6,     /*   83 */
     4,     1,     1,     6,     /*   84 */
     4,     1,     1,     6,     /*   85 */
    65,    18,    17,   100,     /*   86 */
    95,    29,    26,   150,     /*   87 */
   185,    62,    53,   300,     /*   88 */
    30,    11,     9,    50,     /*   89 */
    35,    14,    11,    60,     /*   90 */
    85,    37,    28,   150,     /*   91 */
    55,    26,    19,   100,     /*   92 */
    80,    41,    29,   150,     /*   93 */
   155,    86,    59,   300,     /*   94 */
     5,     3,     2,    10,     /*   95 */
     5,     3,     2,    10,     /*   96 */
     5,     3,     2,    10,     /*   97 */
     5,     3,     2,    10,     /*   98 */
     5,     3,     2,    10,     /*   99 */
     5,     3,     2,    10,     /*  100 */
     5,     3,     2,    10,     /*  101 */
     5,     3,     2,    10,     /*  102 */
     5,     3,     2,    10,     /*  103 */
     5,     3,     2,    10,     /*  104 */
     5,     3,     2,    10,     /*  105 */
     5,     3,     2,    10,     /*  106 */
     5,     3,     2,    10,     /*  107 */
   305,   176,   119,   600,     /*  108 */
   155,    86,    59,   300,     /*  109 */
   105,    56,    39,   200,     /*  110 */
    80,    41,    29,   150,     /*  111 */
    65,    32,    23,   120,     /*  112 */
    55,    26,    19,   100,     /*  113 */
   335,   152,   113,   600,     /*  114 */
    85,    37,    28,   150,     /*  115 */
   115,    48,    37,   200,     /*  116 */
    35,    14,    11,    60,     /*  117 */
   355,   136,   109,   600,     /*  118 */
    30,    11,     9,    50,     /*  119 */
   365,   128,   107,   600,     /*  120 */
   185,    62,    53,   300,     /*  121 */
    25,     8,     7,    40,     /*  122 */
    95,    29,    26,   150,     /*  123 */
   385,   112,   103,   600,     /*  124 */
    65,    18,    17,   100,     /*  125 */
   395,   104,   101,   600,     /*  126 */
     4,     1,     1,     6,     /*  127 */
     4,     1,     1,     6,     /*  128 */
   395,   104,   101,   600,     /*  129 */
    65,    18,    17,   100,     /*  130 */
   385,   112,   103,   600,     /*  131 */
    95,    29,    26,   150,     /*  132 */
    25,     8,     7,    40,     /*  133 */
   185,    62,    53,   300,     /*  134 */
   365,   128,   107,   600,     /*  135 */
    30,    11,     9,    50,     /*  136 */
   355,   136,   109,   600,     /*  137 */
    35,    14,    11,    60,     /*  138 */
   115,    48,    37,   200,     /*  139 */
    85,    37,    28,   150,     /*  140 */
   335,   152,   113,   600,     /*  141 */
    55,    26,    19,   100,     /*  142 */
    65,    32,    23,   120,     /*  143 */
    80,    41,    29,   150,     /*  144 */
   105,    56,    39,   200,     /*  145 */
   155,    86,    59,   300,     /*  146 */
   305,   176,   119,   600,     /*  147 */
     5,     3,     2,    10,     /*  148 */
     5,     3,     2,    10,     /*  149 */
     5,     3,     2,    10,     /*  150 */
     5,     3,     2,    10,     /*  151 */
     5,     3,     2,    10,     /*  152 */
     5,     3,     2,    10,     /*  153 */
     5,     3,     2,    10,     /*  154 */
     5,     3,     2,    10,     /*  155 */
     5,     3,     2,    10,     /*  156 */
     5,     3,     2,    10,     /*  157 */
     5,     3,     2,    10,     /*  158 */
     5,     3,     2,    10,     /*  159 */
     5,     3,     2,    10,     /*  160 */
   155,    86,    59,   300,     /*  161 */
    80,    41,    29,   150,     /*  162 */
    55,    26,    19,   100,     /*  163 */
    85,    37,    28,   150,     /*  164 */
    35,    14,    11,    60,     /*  165 */
    30,    11,     9,    50,     /*  166 */
   185,    62,    53,   300,     /*  167 */
    95,    29,    26,   150,     /*  168 */
    65,    18,    17,   100,     /*  169 */
     4,     1,     1,     6,     /*  170 */
     4,     1,     1,     6,     /*  171 */
     4,     1,     1,     6,     /*  172 */
     4,     1,     1,     6,     /*  173 */
     4,     1,     1,     6,     /*  174 */
     4,     1,     1,     6,     /*  175 */
     4,     1,     1,     6,     /*  176 */
     4,     1,     1,     6,     /*  177 */
     4,     1,     1,     6,     /*  178 */
   119,    47,    29,   195,     /*  179 */
    72,    41,    17,   130,     /*  180 */
    97,    76,    22,   195,     /*  181 */
   172,   181,    37,   390,     /*  182 */
     5,     7,     1,    13,     /*  183 */
    83,   111,    14,   208,     /*  184 */
    43,    55,     6,   104,     /*  185 */
    89,   109,    10,   208,     /*  186 */
    23,    27,     2,    52,     /*  187 */
    95,   107,     6,   208,     /*  188 */
    49,    53,     2,   104,     /*  189 */
   101,   105,     2,   208,     /*  190 */
     1,     1,     0,     2,     /*  191 */
   307,   303,     6,   616,     /*  192 */
   153,   149,     6,   308,     /*  193 */
   305,   293,    18,   616,     /*  194 */
    38,    36,     3,    77,     /*  195 */
   303,   283,    30,   616,     /*  196 */
   151,   139,    18,   308,     /*  197 */
    43,    39,     6,    88,     /*  198 */
    75,    67,    12,   154,     /*  199 */
   299,   263,    54,   616,     /*  200 */
   149,   129,    30,   308,     /*  201 */
    27,    23,     6,    56,     /*  202 */
    37,    31,     9,    77,     /*  203 */
   295,   243,    78,   616,     /*  204 */
    21,    17,     6,    44,     /*  205 */
   293,   233,    90,   616,     /*  206 */
    73,    57,    24,   154,     /*  207 */
   291,   223,   102,   616,     /*  208 */
   145,   109,    54,   308,     /*  209 */
   289,   213,   114,   616,     /*  210 */
    36,    26,    15,    77,     /*  211 */
    41,    29,    18,    88,     /*  212 */
    13,     9,     6,    28,     /*  213 */
   285,   193,   138,   616,     /*  214 */
    71,    47,    36,   154,     /*  215 */
   283,   183,   150,   616,     /*  216 */
   141,    89,    78,   308,     /*  217 */
   281,   173,   162,   616,     /*  218 */
     5,     3,     3,    11,     /*  219 */
   423,   256,   245,   924,     /*  220 */
   213,   130,   119,   462,     /*  221 */
    13,     8,     7,    28,     /*  222 */
   108,    67,    56,   231,     /*  223 */
   435,   272,   217,   924,     /*  224 */
    73,    46,    35,   154,     /*  225 */
    63,    40,    29,   132,     /*  226 */
   111,    71,    49,   231,     /*  227 */
   149,    96,    63,   308,     /*  228 */
   225,   146,    91,   462,     /*  229 */
   453,   296,   175,   924,     /*  230 */
    38,    25,    14,    77,     /*  231 */
   459,   304,   161,   924,     /*  232 */
     3,     2,     1,     6,     /*  233 */
   471,   304,   161,   936,     /*  234 */
   237,   148,    83,   468,     /*  235 */
    53,    32,    19,   104,     /*  236 */
    60,    35,    22,   117,     /*  237 */
   483,   272,   181,   936,     /*  238 */
    81,    44,    31,   156,     /*  239 */
   489,   256,   191,   936,     /*  240 */
   123,    62,    49,   234,     /*  241 */
   165,    80,    67,   312,     /*  242 */
   249,   116,   103,   468,     /*  243 */
   501,   224,   211,   936,     /*  244 */
     7,     3,     3,    13,     /*  245 */
    43,    15,    20,    78,     /*  246 */
    22,     6,    11,    39,     /*  247 */
    15,     3,     8,    26,     /*  248 */
    23,     3,    13,    39,     /*  249 */
    47,     3,    28,    78,     /*  250 */
     8,     0,     5,    13,     /*  251 */
     7,     0,     4,    11,     /*  252 */
    21,     0,    10,    31,     /*  253 */
    13,     0,     5,    18,     /*  254 */
    13,     0,     5,    18      /*  255 */
};
