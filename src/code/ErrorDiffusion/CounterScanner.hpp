/*
  CounterScanner.hpp

  general scanner based on a counter class

  Li-Yi Wei
  05/01/2009

*/

#ifndef _COUNTER_SCANNER_HPP
#define _COUNTER_SCANNER_HPP

#include "Scanner.hpp"
#include "Counter.hpp"
#include "SequentialCounter.hpp"

class CounterScanner : public Scanner
{
public:
    CounterScanner(void);
    virtual ~CounterScanner(void) = 0;

    virtual int Reset(void);

    virtual int GetCoordinate(Coordinate & coordinate) const;

    virtual int GetOffset(const Coordinate & input, Coordinate & output) const;

    virtual int Next(void);

protected:
    virtual Counter & GetCounter(void);
    virtual const Counter & GetCounter(void) const;

protected:
    mutable vector<int> _value;
    SequentialCounter _dummy;
};

#endif
