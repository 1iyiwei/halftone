/*
  SerpentineScanner.hpp

  Li-Yi Wei
  04/24/2009

*/

#ifndef _SERPENTINE_SCANNER_HPP
#define _SERPENTINE_SCANNER_HPP

#include "CounterScanner.hpp"
#include "SequentialCounter.hpp"

class SerpentineScanner : public CounterScanner
{
public:
    SerpentineScanner(const vector<int> & digit_min, const vector<int> & digit_max);
    ~SerpentineScanner(void);

    int GetCoordinate(Coordinate & coordinate) const;

    int GetOffset(const Coordinate & input, Coordinate & output) const;

protected:
    Counter & GetCounter(void);
    const Counter & GetCounter(void) const;

protected:
    SequentialCounter _counter;
    const vector<int> _digit_min, _digit_max;
};

#endif
