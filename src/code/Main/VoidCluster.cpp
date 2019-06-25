/*
  VoidCluster.cpp

  Li-Yi Wei
  05/05/2009

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include <stdlib.h>
#include <math.h>

#include "DiscreteVoidCluster.hpp"
#include "GridRegularVoidCluster.hpp"
#include "GridJitterVoidCluster.hpp"
#include "WhiteRandomVoidCluster.hpp"

#include "FairClassWeight.hpp"
#include "RainbowClassWeight.hpp"

#include "Random.hpp"
#include "RandomCounter.hpp"
#include "FrameBuffer.hpp"
#include "Exception.hpp"

// #define SHOW_PROGRESS
// #define SHOW_VALUE
// #define SHOW_PEAK_TROUGH

vector<FrameBuffer::P3> BuildPalette(const int num_colors, const string color_palette_file_name)
{
    vector<FrameBuffer::P3> palette(num_colors);

    const FrameBuffer::P3 fixed_palette[] = 
        {
            {255, 0, 0}, {0, 153, 0}, {0, 0, 255},
            {0, 255, 255}, {255, 0, 255}, {255, 255, 0}
        };

    vector<FrameBuffer::P3> given_palette;

    if(color_palette_file_name == "")
    {
        for(int i = 0; i < sizeof(fixed_palette)/sizeof(FrameBuffer::P3); i++)
        {
            given_palette.push_back(fixed_palette[i]);
        }
    }
    else
    {
        ifstream input(color_palette_file_name.c_str());

        if(! input)
        {
            cerr << "cannot read from " << color_palette_file_name << endl;
        }
        else
        {
            FrameBuffer::P3 color;
            while(input.good())
            {
                input >> color.r;
                input >> color.g;
                input >> color.b;

                if(input.good()) given_palette.push_back(color);
            }
        }
    }

    // cerr << "given color: " << given_palette.size() << ", total color: " << num_colors << endl;

    for(unsigned int i = 0; i < palette.size(); i++)
    {
        if(i < given_palette.size())
        {
            palette[i] = given_palette[i];
        }
        else
        {
            palette[i].r = static_cast<int>(floor(256*Random::UniformRandom()));
            palette[i].g = static_cast<int>(floor(256*Random::UniformRandom()));
            palette[i].b = static_cast<int>(floor(256*Random::UniformRandom()));
        }
    }

    return palette;
}

int Main(int argc, char **argv)
{
    // input arguments
    const int argc_min = 10;
    if(argc < argc_min)
    {
        cerr << "Usage: " << argv[0] << " method (discrete, gridregular, gridjiter, whiterandom) grid_size num_classes num_samples_per_class (num_classes integers) class_weighting_scheme (fair, rainbow) relative_splat_size_float init_swap_size_int [for gridjitter: candidate_size relative_jitter_radius] min_num_iterations output_file_name [optional: color_palette_file_name]" << endl;
        return 1;
    }

    int arg_ctr = 0;
    const string method = argv[++arg_ctr];
    const int dimension = 2;
    const int grid_size_1d = atoi(argv[++arg_ctr]);
    const int num_classes = atoi(argv[++arg_ctr]);

    if((grid_size_1d <= 0) || (num_classes <= 0))
    {
        cerr << "illegal non-positive input arguments" << endl;
        return 1;
    }

    if(argc < (argc_min + num_classes - 1))
    {
        cerr << "not enough input arguments" << endl;
        return 1;
    }

    const vector<int> grid_size(dimension, grid_size_1d);

    vector<int> num_samples(num_classes);
    for(unsigned int i = 0; i < num_samples.size(); i++)
    {
        num_samples[i] = atoi(argv[++arg_ctr]);
    }

    const string class_weighting_scheme = argv[++arg_ctr];
    const float relative_splat_size = atof(argv[++arg_ctr]);
    const int init_swap_size = atoi(argv[++arg_ctr]);
    
    if(method == "gridjitter")
    {
        if(argc < (argc_min+2))
        {
            cerr << "not enough arguments for grid jitter" << endl;
            return 1;
        }
    }
 
    if(method == "whiterandom")
    {
        if(argc < (argc_min+1))
        {
            cerr << "not enough arguments for white random" << endl;
            return 1;
        }
    }

    const int trough_candidate_size = ((arg_ctr < argc-1) && ((method == "gridjitter") || (method == "whiterandom")) ? atoi(argv[++arg_ctr]) : -1);
    const float trough_jitter_radius = ((arg_ctr < argc-1) && (method == "gridjitter") ? atof(argv[++arg_ctr])/grid_size_1d : -1);

    const int min_num_iterations = atoi(argv[++arg_ctr]);
    const string output_file_name = argv[++arg_ctr];
    const string color_palette_file_name = (arg_ctr < argc-1 ? argv[++arg_ctr] : "");

    for(unsigned int i = 0; i < num_samples.size(); i++)
    {
        if(num_samples[i] <= 0)
        {
            cerr << "number of samples per class must be > 0" << endl;
            return 1;
        }
    }

    // generate initial samples
    Random::InitRandomNumberGenerator();

    vector<VoidCluster::Sample> samples;

    if(method == "discrete")
    {
        vector<int> counter_max(grid_size);
        for(unsigned int i = 0; i < counter_max.size(); i++)
        {
            counter_max[i] = grid_size[i]-1;
        }

        RandomCounter counter(dimension, vector<int>(dimension, 0), counter_max);

        counter.Reset();

        VoidCluster::Sample sample;
        sample.position = VoidCluster::Sample::Position(dimension);
        vector<int> index(dimension);

        int active_class = 0;
        int num_samples_for_active_class = 0;
        do
        {
            counter.Get(index);

            for(int i = 0; i < sample.position.Dimension(); i++)
            {
                sample.position[i] = index[i];
            }

            sample.id = active_class;
            sample.value = 1.0;

            samples.push_back(sample);
            
            num_samples_for_active_class++;

            if(num_samples_for_active_class >= num_samples[active_class])
            {
                num_samples_for_active_class = 0;
                active_class++;
            }
        }
        while((active_class < num_classes) && counter.Next());

        if(active_class < num_classes)
        {
            cerr << "target number of samples >= grid capacity" << endl;
            return 1;
        }
    }
    else
    {
        VoidCluster::Sample sample(dimension);

        for(unsigned int which_class = 0; which_class < num_samples.size(); which_class++)
        {
            for(int which_sample = 0; which_sample < num_samples[which_class]; which_sample++)
            {
                for(int i = 0; i < sample.position.Dimension(); i++)
                {
                    sample.position[i] = Random::UniformRandom()*grid_size[i];
                }

                sample.id = which_class;
                sample.value = 1.0;

                samples.push_back(sample);
            }
        }
    }

    // class weight
    ClassWeight * p_class_weight = 0;
    if(class_weighting_scheme == "fair")
    {
        p_class_weight = new FairClassWeight(num_samples);
    }
    else if(class_weighting_scheme == "rainbow")
    {
        p_class_weight = new RainbowClassWeight(num_samples);
    }
    else
    {
        cerr << "unknown class weighing scheme" << endl;
        return 1;
    }

    if(!p_class_weight)
    {
        cerr << "empty class weight" << endl;
        return 1;
    }

    const ClassWeight & class_weight = *p_class_weight;

    // void cluster
    VoidCluster * p_void_cluster = 0;
    if(method == "discrete")
    {
        p_void_cluster = new DiscreteVoidCluster(grid_size, class_weight, relative_splat_size, init_swap_size);
    }
    else if(method == "gridregular")
    {
        p_void_cluster = new GridRegularVoidCluster(grid_size, class_weight, relative_splat_size, init_swap_size);
    }
    else if(method == "gridjitter")
    {
        if(trough_candidate_size <= 0)
        {
            cerr << "illegal trough candidate size" << endl;
            return 1;
        }

        if(trough_jitter_radius < 0)
        {
            cerr << "illegal trough jitter radius" << endl;
            return 1;
        }

        p_void_cluster = new GridJitterVoidCluster(grid_size, class_weight, relative_splat_size, init_swap_size, trough_candidate_size, trough_jitter_radius);
    }
    else if(method == "whiterandom")
    {
        if(trough_candidate_size <= 0)
        {
            cerr << "illegal trough candidate size" << endl;
            return 1;
        }

        p_void_cluster = new WhiteRandomVoidCluster(VoidCluster::Convert(grid_size), class_weight, relative_splat_size, init_swap_size, trough_candidate_size);
    }
    else
    {
        cerr << "unknown method option" << endl;
        return 1;
    }

    if(! p_void_cluster)
    {
        cerr << "empty void cluster" << endl;
        return 1;
    }

    VoidCluster & void_cluster = *p_void_cluster;
    
    if(void_cluster.Init(samples) != VoidCluster::OK)
    {
        cerr << "failure in initializing void cluster" << endl;
        return 1;
    }

    VoidCluster::RunStatus status = VoidCluster::FAILURE;
    int total_num_iterations = 0;
    do
    {
        status = void_cluster.Next(samples);
        // total_num_iterations += (status == VoidCluster::OK);
        total_num_iterations++;
#ifdef SHOW_PROGRESS
        cerr << ".";
#endif
    }
    while((status == VoidCluster::OK) || (total_num_iterations < min_num_iterations));
#ifdef SHOW_PROGRESS
    cerr << endl;
#endif
     
    // check output
    for(unsigned int i = 0; i < samples.size(); i++)
    {
        const VoidCluster::Sample & sample = samples[i];
        
        if((sample.id < 0) || (sample.id >= num_classes))
        {
            cerr << "illegal sample id" << endl;
            return 1;
        }

        if(sample.position.Dimension() != grid_size.size())
        {
            cerr << "wrong sample dimension" << endl;
            return 1;
        }

        for(int j = 0; j < sample.position.Dimension(); j++)
        {
            if((sample.position[j] < 0) || (sample.position[j] >= grid_size[j]))
            {
                cerr << "sample out of bound" << endl;
                return 1;
            }
        }
    }

    // output samples
    cerr << samples.size() << " samples in " << total_num_iterations << " iterations" << endl;

    if(output_file_name == "cout")
    {
        for(unsigned int i = 0; i < samples.size(); i++)
        {
            const VoidCluster::Sample & sample = samples[i];

            cout << sample.id;
            for(int j = 0; j < sample.position.Dimension(); j++)
            {
                cout << " " << ((method == "discrete" || method == "gridregular") ? sample.position[j]+0.5 : sample.position[j])/grid_size[j];
            }
            cout << endl;
        }
    }
    else
    {
        // to a ppm file
        const vector<FrameBuffer::P3> color_palette = BuildPalette(num_classes, color_palette_file_name);
        
        const FrameBuffer::P3 blank = {0, 0, 0};
        Array<FrameBuffer::P3> image(grid_size);
        image.Clear(blank);
        
#ifdef SHOW_VALUE
        {
            // debug info
            Array<double> value;
            if(! void_cluster.Value(value))
            {
                cerr << "cannot get value" << endl;
                return 1;
            }

            vector<double> value_data; value.Get(value_data);
            double max_value = -1; double min_value = 1.0/sin(0.0);

            for(unsigned int i = 0; i < value_data.size(); i++)
            {
                if(max_value < value_data[i]) max_value = value_data[i];
                if(min_value > value_data[i]) min_value = value_data[i];
            }

            if(min_value < 0)
            {
                cerr << "negative minimum value" << endl;
                return 1;
            }

            vector<FrameBuffer::P3> pixel_data(value_data.size());
            for(unsigned int i = 0; i < pixel_data.size(); i++)
            {
                pixel_data[i].r = pixel_data[i].g = pixel_data[i].b = value_data[i]*255/max_value;
            }

            image.Put(pixel_data);
        }
#endif
        vector<int> index(dimension);

        for(unsigned int i = 0; i < samples.size(); i++)
        {
            const VoidCluster::Sample & sample = samples[i];

            for(unsigned int j = 0; j < index.size(); j++)
            {
                index[j] = sample.position[j];
            }

            if(! image.Put(index, color_palette[sample.id]))
            {
                cerr << "cannot put image pixel" << endl;
                return 1;
            }
        }

#ifdef SHOW_PEAK_TROUGH
        {
            // by inverting color channels

            vector< vector<int> > peaks, troughs;

            const int show_class = 0; // which class to show
            if(! void_cluster.Find(show_class, init_swap_size, peaks, troughs))
            {
                cerr << "cannot find peaks and troughs" << endl;
                return 1;
            }

            vector< vector< vector<int> > > extremes;
            extremes.push_back(peaks);
            extremes.push_back(troughs);

            FrameBuffer::P3 value;

            // port: peak or trough
            for(unsigned int port = 0; port < extremes.size(); port++)
            {
                for(unsigned int i = 0; i < extremes[port].size(); i++)
                {
                    if(! image.Get(extremes[port][i], value))
                    {
                        cerr << "cannot get extreme color" << endl;
                        return 1;
                    }

                    value.r = 255-value.r;
                    value.g = 255-value.g;
                    value.b = (port ? 255-value.b : value.b/2);

                    if(! image.Put(extremes[port][i], value))
                    {
                        cerr << "cannot put extreme color" << endl;
                        return 1;
                    }
                }
            }
        }
#endif

        if(! FrameBuffer::WritePPM(image, 255, output_file_name.c_str()))
        {
            cerr << "cannot write to " << output_file_name << endl;
            return 1;
        }
    }

    // clean up
    if(p_class_weight)
    {
        delete p_class_weight;
        p_class_weight = 0;
    }

    if(p_void_cluster)
    {
        delete p_void_cluster;
        p_void_cluster = 0;
    }

    // done
    return 0;
}

int main(int argc, char **argv)
{
    try
    {
        return Main(argc, argv);
    }
    catch(Exception e)
    {
        cerr << "Error : " << e.Message() << endl;
        return 1;
    }
}
