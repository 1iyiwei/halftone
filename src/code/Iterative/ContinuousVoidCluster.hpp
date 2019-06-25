/*
  ContinuousVoidCluster.hpp

  the base class for void/cluster method that runs over a continuous domain

  Li-Yi Wei
  05/10/2009

*/

#ifndef _CONTINUOUS_VOID_CLUSTER_HPP
#define _CONTINUOUS_VOID_CLUSTER_HPP

#include "VoidCluster.hpp"
#include "Array.hpp"
#include "ClassWeight.hpp"

class ContinuousVoidCluster : public VoidCluster
{
public:
    ContinuousVoidCluster(const vector<double> & domain_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size);
    virtual ~ContinuousVoidCluster(void) = 0;

    virtual RunStatus Init(const vector<Sample> & input);
    virtual RunStatus Next(vector<Sample> & output);

    virtual int Find(const int peak_class_id, const int swap_size, vector<Sample::Position> & peaks, vector<Sample::Position> & troughs) const;

protected:
    static int Scatter(const vector<double> & domain_size, const vector<Sample> & input, const vector<float> & weight, const float splat_size, vector<Datum> & output);

    static int Scatter(const vector<double> & domain_size, const vector<Sample> & input, const ClassWeight & class_weight, const int main_class_id, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first, vector<Datum> & output); 

    // class specific behavior
    virtual int Scatter(const vector<double> & domain_size, const vector<Sample> & input, const ClassWeight & class_weight, const int main_class_id, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first);

    // notice the asymmetry between peaks and troughs in the continuous setting
    int Find(const vector<Datum> & potential_peaks, const int peak_class_id, const int swap_size, vector<Datum> & peaks, vector<Sample::Position> & troughs) const;

    // Find() consists of the following
    static int FindPeaks(const vector<Datum> & input, const int peak_class_id, const int swap_size, vector<Datum> & peaks);

    // child-class specific behavior
    virtual int FindTroughs(const int peak_class_id, const int swap_size, vector<Sample::Position> & troughs) const;

    static int Swap(const vector<Datum> & peaks, const vector<Sample::Position> & troughs);

    // compute the bumpiness/variance of grid (the lower, the better)
    static double Energy(const vector<Datum> & bag);

protected:
    const vector<double> _domain_size; // no grid, so has to specify this to know the relative size of the domain to the samples

    vector<Datum> _bag; // for holding the samples
};

#endif
