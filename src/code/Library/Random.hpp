/*
  Random.hpp

  Li-Yi Wei
  06/28/2007

*/

#ifndef _RANDOM_HPP
#define _RANDOM_HPP

class Random
{
public:
    static void InitRandomNumberGenerator(void);
    static void InitRandomNumberGenerator(const unsigned long seed);
    
    // a uniform random number between [0 1]
    static double UniformRandom(void);

    // a Gaussian with 0 mean and 1 variance
    static double GaussianRandom(void);

    // Box-Muller transform
    static void BoxMuller(const double u1, const double u2, double & n1, double & n2); 
};

#endif
