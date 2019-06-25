/*
  DiscreteVoidCluster.hpp

  the classical void/cluster method that runs over a discrete grid

  Li-Yi Wei
  05/03/2009

*/

#ifndef _DISCRETE_VOID_CLUSTER_HPP
#define _DISCRETE_VOID_CLUSTER_HPP

#include "VoidCluster.hpp"
#include "Array.hpp"
#include "ClassWeight.hpp"

class DiscreteVoidCluster : public VoidCluster
{
public:
    // I believe init_swap_size affects mostly speed, not quality
    // so let it be an absolute value for now (instead of relative # samples)
    DiscreteVoidCluster(const vector<int> & grid_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size);
    ~DiscreteVoidCluster(void);

    RunStatus Init(const vector<Sample> & input);
    RunStatus Next(vector<Sample> & output);

    // for debug
    virtual int Value(Array<double> & value) const;

    virtual int Find(const int peak_class_id, const int swap_size, vector<Sample::Position> & peaks, vector<Sample::Position> & troughs) const;

protected:
    // the individual steps for Next()

    // mainly affects Datum.sample but also clears out Datum.value
    static int Scatter(const vector<Sample> & input, Array<Datum> & grid);

    // peaks have to be grid cells containing samples with peak_class_id
    // troughs have to be grid cells without samples
    static int Find(const Array<Datum> & grid, const int peak_class_id, const int swap_size, vector< vector<int> > & peaks, vector< vector<int> > & troughs);

    // changes only grid datum.sample, but not grid datum.value
    // be very careful on this
    static int Swap(const vector< vector<int> > & peaks, const vector< vector<int> > & troughs, Array<Datum> & grid);

    // collect the set of samples from grid
    // no need for this function since I keep _samples explicitly
    // static int Gather(const Array<Datum> & grid, vector<Sample> & output);

    // mean and variance of grid data values
    static double Mean(const Array<Datum> & grid);
    static double Variance(const Array<Datum> & grid);

    // compute the bumpiness/variance of grid (the lower, the better)
    static double Energy(const Array<Datum> & grid);

protected:
    Array<Datum> _grid;
};

#endif
