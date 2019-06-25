/*
  WhiteRandomVoidCluster.hpp

  use white random for picking potential trough locations

  Li-Yi Wei
  05/18/2009

*/

#ifndef _WHITE_RANDOM_VOID_CLUSTER_HPP
#define _WHITE_RANDOM_VOID_CLUSTER_HPP

#include "ContinuousVoidCluster.hpp"

class WhiteRandomVoidCluster : public ContinuousVoidCluster
{
public:
    WhiteRandomVoidCluster(const vector<double> & domain_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size, const int candidate_size);

    virtual ~WhiteRandomVoidCluster(void);

    virtual int Value(Array<double> & value) const;

protected:
    virtual int Scatter(const vector<double> & domain_size, const vector<Sample> & input, const ClassWeight & class_weight, const int main_class_id, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first);

    virtual int FindTroughs(const int peak_class_id, const int swap_size, vector<Sample::Position> & troughs) const;

    static int Dart(const vector<double> & domain_size, Sample::Position & output);
    
protected:
    const int _candidate_size;

    mutable vector<Datum> _trough_candidates;
    mutable vector<Sample> _trough_samples;

};

#endif
