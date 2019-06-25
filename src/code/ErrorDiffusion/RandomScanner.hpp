/*
  RandomScanner.hpp

  Li-Yi Wei
  04/24/2009

*/

#ifndef _RANDOM_SCANNER_HPP
#define _RANDOM_SCANNER_HPP

#include "Scanner.hpp"
#include "RandomCounter.hpp"

class RandomScanner : public Scanner
{
public:
    RandomScanner(const vector<int> & digit_min, const vector<int> & digit_max);
    ~RandomScanner(void);

    int Reset(void);

    int GetCoordinate(Coordinate & coordinate) const;

    int GetOffset(const Coordinate & input, Coordinate & output) const;

    int Next(void);

protected:
    RandomCounter _counter;
    mutable vector<int> _value;
};

#endif
