/*
  HilbertScanner.hpp

  Li-Yi Wei
  05/01/2009

*/

#ifndef _HILBERT_SCANNER_HPP
#define _HILBERT_SCANNER_HPP

#include "CounterScanner.hpp"
#include "HilbertCounter.hpp"

class HilbertScanner : public CounterScanner
{
public:
    HilbertScanner(const vector<int> & digit_min, const vector<int> & digit_max);
    ~HilbertScanner(void);

    int GetOffset(const Coordinate & input, Coordinate & output) const;

protected:
    Counter & GetCounter(void);
    const Counter & GetCounter(void) const;

protected:
    HilbertCounter _counter;
};

#endif
