/*
  VoidCluster.hpp

  void and cluster method for discrete joint blue noise

  Li-Yi Wei
  05/03/2009

*/

#ifndef _VOID_CLUSTER_HPP
#define _VOID_CLUSTER_HPP

#include <vector>
using namespace std;

#include "Coordinate.hpp"
#include "Array.hpp"
#include "ClassWeight.hpp"

class VoidCluster
{
public: 
    struct Sample
    {
        Sample(void) : id(0), value(1.0) {};
        Sample(const int dimension) : id(0), position(dimension), value(1.0) {};
        int id;
        typedef CoordinateT<double> Position;
        Position position; // use double so that I could have enough precision to handle both integer and floating point situations
        double value; // should be scalar for picking void/cluster center
    };

    VoidCluster(const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size);
    virtual ~VoidCluster(void) = 0;

    enum RunStatus {OK, FAILURE, DONE};

    // initialize the input samples
    virtual RunStatus Init(const vector<Sample> & input);

    // one iteration
    virtual RunStatus Next(vector<Sample> & output);

    // for debug and visualization
    virtual int Value(Array<double> & value) const;
    virtual int Find(const int peak_class_id, const int swap_size, vector<Sample::Position> & peaks, vector<Sample::Position> & troughs) const;

public:
    static int NumSamples(const vector<Sample> & samples, vector<int> & class_num_samples);

    static vector<double> Convert(const vector<int> & input);
    static vector<int> Convert(const vector<double> & input);
    static void Convert(const vector<double> & input, Sample::Position & output);

protected:
    // radius of the splat size estimated from grid size and number of samples
    static float SplatSize(const vector<int> & grid_size, const int num_samples);

    // estimate the uniform splat size for all classes
    static float SplatSize(const vector<int> & grid_size, const vector<int> & num_samples);

    // similar to above, but use double domain size
    static float SplatSize(const vector<double> & domain_size, const int num_samples);
    static float SplatSize(const vector<double> & domain_size, const vector<int> & num_samples);

    // the common data structure for holding accumulated splatting values
    // used for both discrete and continuous settings
    // (for continuous setting, the sample pointer should never be 0)
    // (but I use the same data structure for semantic clarity)
    struct Datum
    {
        Datum(void) : value(0), sample(0) {};

        double value;
        const Sample * sample;
    };

    // all vector arrays should contain all classes
    // for classes not used, simply zero out weighting
    // weighting should be specified to avoid complexity in tuning input values
    // only affects Datum.value but not Datum.sample
    static int Scatter(const vector<Sample> & input, const vector<float> & weight, const float splat_size, Array<Datum> & grid);

    // meta function for above, handling a single target class
    // under differet scenarios (initial scatter, before/after swap)
    static int Scatter(const vector<Sample> & input, const ClassWeight & class_weight, const int main_class_id, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first, Array<Datum> & grid);

    struct IndexDatum // for convenience in Find()
    {
        vector<int> index; // my location in the domain
        Datum data;
    };

    // most core routines
    static int FindPeaks(const vector<IndexDatum> & input, const int peak_class_id, const int swap_size, vector<IndexDatum> & peaks);     
    static int FindTroughs(const vector<IndexDatum> & input, const int swap_size, vector<IndexDatum> & troughs);

    // wrapper for grid input
    static int FindPeaks(const Array<Datum> & grid, const int peak_class_id, const int swap_size, vector< vector<int> > & peaks);
    static int FindTroughs(const Array<Datum> & grid, const int swap_size, vector< vector<int> > & troughs);

    // convenience routine if grid is used
    static int Value(const Array<Datum> & input, Array<double> & output);

protected:
    const ClassWeight & _class_weight;
    const float _relative_splat_size;
    const int _init_swap_size;
    
    int _swap_size;

    vector<Sample> _samples;
    vector<int> _num_samples; // per class
};
#endif
