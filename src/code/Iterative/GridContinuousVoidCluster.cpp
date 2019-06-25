/*
  GridContinuousVoidCluster.cpp

  Li-Yi Wei
  05/15/2009

*/

#include "GridContinuousVoidCluster.hpp"
#include "Exception.hpp"
  
GridContinuousVoidCluster::GridContinuousVoidCluster(const vector<int> & grid_size, const ClassWeight & class_weight, const float relative_splat_size, const int init_swap_size) : ContinuousVoidCluster(Convert(grid_size), class_weight, relative_splat_size, init_swap_size), _grid(grid_size)
{
    // nothing else to do
}

GridContinuousVoidCluster::~GridContinuousVoidCluster(void)
{
    // nothing else to do
}

int GridContinuousVoidCluster::Scatter(const vector<double> & domain_size, const vector<Sample> & input, const ClassWeight & class_weight, const int main_class_id, const vector<int> & class_selection, const float relative_splat_size, const bool clear_first)
{
    if(! ContinuousVoidCluster::Scatter(domain_size, input, class_weight, main_class_id, class_selection, relative_splat_size, clear_first, _bag))
    {
        return 0;
    }

    if(! VoidCluster::Scatter(input, class_weight, main_class_id, class_selection, relative_splat_size, clear_first, _grid))
    {
        return 0;
    }

    // done
    return 1;
}

int GridContinuousVoidCluster::FindTroughs(const int peak_class_id, const int swap_size, vector<Sample::Position> & troughs_out) const
{
    vector< vector<int> > troughs;

    if(! VoidCluster::FindTroughs(_grid, swap_size, troughs))
    {
        return 0;
    }

    troughs_out = vector<Sample::Position>(troughs.size());

    for(unsigned int i = 0; i < troughs_out.size(); i++)
    {
        troughs_out[i] = Sample::Position(troughs[i].size());

        for(int j = 0; j < troughs_out[i].Dimension(); j++)
        {
            troughs_out[i][j] = troughs[i][j];
        }
    }

    return troughs_out.size();
}

int GridContinuousVoidCluster::Value(Array<double> & value) const
{
    return VoidCluster::Value(_grid, value);
}
