/*
  SequentialScanner.hpp

  Li-Yi Wei
  04/24/2009

*/

#ifndef _SEQUENTIAL_SCANNER_HPP
#define _SEQUENTIAL_SCANNER_HPP

#include "CounterScanner.hpp"
#include "SequentialCounter.hpp"

class SequentialScanner : public CounterScanner
{
public:
    SequentialScanner(const vector<int> & digit_min, const vector<int> & digit_max);
    ~SequentialScanner(void);

protected:
    Counter & GetCounter(void);
    const Counter & GetCounter(void) const;

protected:
    SequentialCounter _counter;
};

#endif
