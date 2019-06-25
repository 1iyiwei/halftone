/*
  ErrorDiffuse.cpp

  the main code for error diffusion

  Li-Yi Wei
  04/23/2009

*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <math.h>

#include "Random.hpp"

#include "Utility.hpp"

#include "PlainThresholder.hpp"
#include "ZhouThresholder.hpp"
#include "ConstrainedThresholder.hpp"

#include "ArrayConstrainer.hpp"

#include "FloydSteinbergDistributor.hpp"
#include "VarCoeffDistributor.hpp"
#include "ZhouDistributor.hpp"
#include "KnuthDistributor.hpp"
#include "GoodNeighborDistributor.hpp"

#include "PlainDiffuser.hpp"

#include "FloydSteinbergDiffuser.hpp"
#include "VarCoeffDiffuser.hpp"
#include "ZhouDiffuser.hpp"
#include "KnuthDiffuser.hpp"
#include "KnuthZhouDiffuser.hpp"

#include "GoodNeighborDiffuser.hpp"

#include "ConstrainedThresholder.hpp"
#include "ArrayConstrainer.hpp"

#include "SequentialScanner.hpp"
#include "SerpentineScanner.hpp"
#include "RandomScanner.hpp"
#include "HilbertScanner.hpp"

#include "SequentialCounter.hpp"
#include "Exception.hpp"

// #define TOROIDAL_BOUNDARY // error diffusion propagate toroidally

vector<float> GetRawValues(const string & input)
{
    string buf; // Have a buffer string
    stringstream ss(input); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words

    while(ss >> buf)
    {
        tokens.push_back(buf);
    }

    vector<float> result(tokens.size());

    for(unsigned int i = 0; i < result.size(); i++)
    {
        result[i] = atof(tokens[i].c_str());
    }

    return result;
}

int ReadImage(const string & input_file_name, int & maximum_value, int & color_dimension, Image & input_image)
{
    maximum_value = 0;
    color_dimension = 0;

    Array<FrameBuffer::P2> input_pgm;
    Array<FrameBuffer::P3> input_ppm;

    const vector<float> raw_values = GetRawValues(input_file_name);
    if(raw_values.size() <= 0) throw Exception("cannot read image");

    color_dimension = (raw_values.size() > 1 ? raw_values.size() : (raw_values[0] > 0 ? 1:3));

    const float input_value = fabs(raw_values[0]);

    if(input_value > 0)
    {
        const int image_size = static_cast<int>(floor(input_value));
        const float intensity = input_value - image_size;
        const int dimension = 2;
        maximum_value = 1.0;

        input_image = Image(vector<int>(dimension, image_size));
        Pixel pixel(color_dimension);
        //pixel.Clear(intensity);
        for(int i = 0; i < pixel.Dimension(); i++)
        {
            const float value = (raw_values.size() > 1 ? fabs(raw_values[i]) : input_value);

            pixel[i] = value - floor(value);
        }
        input_image.Clear(pixel);
    }
    else if(FrameBuffer::ReadPGM(input_file_name.c_str(), input_pgm, maximum_value))
    {
        color_dimension = 1;
    
        if(! Utility::Convert(input_pgm, input_image))
        {
            cerr << "cannot convert input image" << endl;
            return 0;
        }
    }
    else if(FrameBuffer::ReadPPM(input_file_name.c_str(), input_ppm, maximum_value))
    {
        color_dimension = 3;
    
        if(! Utility::Convert(input_ppm, input_image))
        {
            cerr << "cannot convert input image" << endl;
            return 0;
        }
    }
    else 
    {
        return 0;
    }

    // done
    return 1;
}

int WriteImage(const Image & output_image, const int maximum_value, const int color_dimension, const string & output_file_name)
{
    if(color_dimension == 1)
    {
        Array<FrameBuffer::P2> output_pgm;
        if(! Utility::Convert(output_image, output_pgm))
        {
            cerr << "cannot convert output image" << endl;
            return 0;
        }
    
        if(! FrameBuffer::WritePGM(output_pgm, maximum_value, output_file_name.c_str()))
        {
            return 0;
        }
    }
    else if(color_dimension == 3)
    {
        Array<FrameBuffer::P3> output_ppm;
        if(! Utility::Convert(output_image, output_ppm))
        {
            cerr << "cannot convert output image" << endl;
            return 0;
        }
    
        if(! FrameBuffer::WritePPM(output_ppm, maximum_value, output_file_name.c_str()))
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    // done
    return 1;
}

// max_pixel encodes both max value and number of colors
int ReadColorPalette(const string & palette_file_name, const vector<int> max_pixel, vector<Pixel> & palette)
{
    const int color_dimension = max_pixel.size();

    palette.clear();
    Pixel pixel(color_dimension);

    ifstream input(palette_file_name.c_str());

    if(input)
    {
        // read palette
        while(input.good())
        {
            for(int i = 0; i < pixel.Dimension(); i++)
            {
                input >> pixel[i];
            }

            if(input.good())
            {
                palette.push_back(pixel);
            }
        }
    }
    else
    {
        // default palette, corners of color hypercube
        SequentialCounter counter(color_dimension, vector<int>(color_dimension, 0), vector<int>(color_dimension, 1));
        
        vector<int> index(color_dimension);

        counter.Reset();
        do
        {
            counter.Get(index);

            Utility::Convert(index, pixel);

            palette.push_back(pixel);
        }
        while(counter.Next());
    }

    // normalize with respect to max_pixel
    for(unsigned int i = 0; i < palette.size(); i++)
    {
        for(int j = 0; j < palette[i].Dimension(); j++)
        {
            palette[i][j] *= max_pixel[j];
        }
    }

    // done
    return 1;
}

string Convert(const Image & constraint_color_image, const Image & constraint_spec_image, Array<ArrayConstrainer::ConstrainedPixel> & constraint_array)
{
    // compatibility check
    if(constraint_color_image.Dimension() != constraint_spec_image.Dimension())
    {
        return "constraint dimensionality mismatch";
    }

    for(int i = 0; i < constraint_color_image.Dimension(); i++)
    {
        if(constraint_color_image.Size(i) != constraint_spec_image.Size(i))
        {
            return "constraint size mismatch";
        }
    }

    // initialization
    vector<int> array_size;
    constraint_color_image.Size(array_size);

    constraint_array = Array<ArrayConstrainer::ConstrainedPixel>(array_size);

    // conversion
    vector<Pixel> constraint_color_pixels, constraint_spec_pixels;
    vector<ArrayConstrainer::ConstrainedPixel> constraint_array_pixels;

    constraint_color_image.Get(constraint_color_pixels);
    constraint_spec_image.Get(constraint_spec_pixels);
    constraint_array.Get(constraint_array_pixels);

    if((constraint_color_pixels.size() != constraint_spec_pixels.size()) || (constraint_color_pixels.size() != constraint_array_pixels.size()))
    {
        return "weird";
    }

    for(unsigned int i = 0; i < constraint_array_pixels.size(); i++)
    {

        constraint_array_pixels[i].value = constraint_color_pixels[i];
        
        int constrained = 0;
        for(int j = 0; j < constraint_spec_pixels[i].Dimension(); j++)
        {
            constrained += constraint_spec_pixels[i][j];
        }

        constraint_array_pixels[i].constrained = (constrained > 0);
    }

    constraint_array.Put(constraint_array_pixels);

    // done
    return "";
}

int Main(int argc, char **argv)
{
    // input arguments
#ifdef CONSTRAINED
    const int min_argc = 8;
#else
    const int min_argc = 6;
#endif

    if(argc < min_argc)
    {
        cerr << "Usage: " << argv[0] << " input_file_name (or a floating point for constant image, where the integer part specifies image size and the fraction part intensity) output_file_name diffuser_type scanner_type color_palette_file_name";

#ifdef CONSTRAINED
        cerr << " constraint_color_file_name constraint_spec_file_name";
#endif
        cerr << endl;
        return 1;
    }

    int arg_ctr = 0;
    const string input_file_name = argv[++arg_ctr];
    const string output_file_name = argv[++arg_ctr];
    const string diffuser_type = argv[++arg_ctr];
    const string scanner_type = argv[++arg_ctr];
    const string color_palette_file_name = argv[++arg_ctr];
#ifdef CONSTRAINED
    const string constraint_color_file_name = argv[++arg_ctr];
    const string constraint_spec_file_name = argv[++arg_ctr];
#endif

#ifdef TOROIDAL_BOUNDARY
    const GoodNeighborDistributor::BoundaryCondition boundary_condition = GoodNeighborDistributor::BOUNDARY_TOROIDAL;
#else
    const GoodNeighborDistributor::BoundaryCondition boundary_condition = GoodNeighborDistributor::BOUNDARY_NONE;
#endif

    // read input image
    int maximum_value = 0;
    int color_dimension = 1;
    Image input_image;

    if(! ReadImage(input_file_name.c_str(), maximum_value, color_dimension, input_image))
    {
        cerr << "cannot read from " << input_file_name << endl;
        return 1;
    }

    vector<int> image_size;
    input_image.Size(image_size);

    // init random number generator
    Random::InitRandomNumberGenerator();

    // palette
    vector<Pixel> palette;
    
    if(! ReadColorPalette(color_palette_file_name, vector<int>(color_dimension, maximum_value), palette))
    {
        cerr << "error in reading color palette" << endl;
        return 1;
    }

#ifdef CONSTRAINED
    // constraint color
    int constraint_color_maximum_value = 0;
    int constraint_color_dimension = 1;
    Image constraint_color_image;
    
    if(! ReadImage(constraint_color_file_name.c_str(), constraint_color_maximum_value, constraint_color_dimension, constraint_color_image))
    {
        cerr << "cannot read from " << constraint_color_file_name << endl;
        return 1;
    }

#if 0
    if(constraint_color_maximum_value != maximum_value)
    {
        cerr << "incompatible maximum value" << endl;
        return 1;
    }
#endif
    if(constraint_color_dimension != color_dimension)
    {
        cerr << "incompatible color dimension" << endl;
        return 1;
    }

    // constraint spec
    int constraint_spec_maximum_value = 0;
    int constraint_spec_color_dimension = 1;
    Image constraint_spec_image;
    
    if(! ReadImage(constraint_spec_file_name.c_str(), constraint_spec_maximum_value, constraint_spec_color_dimension, constraint_spec_image))
    {
        cerr << "cannot read from " << constraint_spec_file_name << endl;
        return 1;
    }

    // compatibility check
    if((constraint_color_image.Dimension() != input_image.Dimension()) || (constraint_color_image.Dimension() != constraint_spec_image.Dimension()))
    {
        cerr << "constraint dimensionality mismatch" << endl;
        return 1;
    }

    for(int i = 0; i < input_image.Dimension(); i++)
    {
        if((constraint_color_image.Size(i) != input_image.Size(i)) || (constraint_color_image.Size(i) != constraint_spec_image.Size(i)))
        {
            cerr << "constraint size mismatch" << endl;
            return 1;
        }
    }
#endif

    // max pixel
    Pixel max_pixel(color_dimension);
    for(int i = 0; i < max_pixel.Dimension(); i++)
    {
        max_pixel[i] = maximum_value;
    }

    // thresholder
    Thresholder * p_thresholder = 0;
    Thresholder * p_super_thresholder = 0;

    if((diffuser_type == "zhou") || (diffuser_type == "knuthzhou"))
    {
        p_thresholder = new ZhouThresholder(palette, max_pixel);
    }
    else
    {
        p_thresholder = new PlainThresholder(palette);
    }

    if(p_thresholder == 0) throw Exception("empty thresholder");

#ifdef CONSTRAINED
    Array<ArrayConstrainer::ConstrainedPixel> constraint_array;
    const string constraint_conversion_message = Convert(constraint_color_image, constraint_spec_image, constraint_array);
    if(constraint_conversion_message != "")
    {
        cerr << "error in constraint conversion: " << constraint_conversion_message << endl;
        return 1;
    }
    ArrayConstrainer constrainer(constraint_array);
    p_super_thresholder = new ConstrainedThresholder(*p_thresholder, constrainer);
#endif

    const Thresholder & thresholder = (p_super_thresholder ? *p_super_thresholder : *p_thresholder);

    // occupancy map
    Array<int> occupancy_map(image_size);
    occupancy_map.Clear(0);

    // distributor
    Distributor * p_distributor = 0;
    Distributor * p_super_distributor = 0;
   
    if(diffuser_type == "floydsteinberg")
    {
        p_distributor = new FloydSteinbergDistributor;
    }
    else if(diffuser_type == "varcoeff")
    {
        p_distributor = new VarCoeffDistributor(max_pixel);
    }
    else if(diffuser_type == "zhou")
    {
        p_distributor = new ZhouDistributor(max_pixel);
    }
    else if((diffuser_type == "knuth") || (diffuser_type == "knuthzhou"))
    {
        p_distributor = new KnuthDistributor;
    }
    else
    {
        cerr << "unsupported diffuser_type" << endl;
        return 1;
    }

    if(p_distributor == 0) throw Exception("empty distributor");

    if((scanner_type == "random") || (scanner_type == "hilbert") || (diffuser_type == "knuth") || (diffuser_type == "knuthzhou"))
    {
        p_super_distributor = new GoodNeighborDistributor(*p_distributor, occupancy_map, boundary_condition);
    }

    const Distributor & distributor = (p_super_distributor ? *p_super_distributor : *p_distributor);

    // diffuser
    const PlainDiffuser diffuser(thresholder, distributor);

    // scanner
    const int dimension = image_size.size();
    vector<int> counter_min = vector<int>(dimension, 0);
    vector<int> counter_max = vector<int>(dimension, 0);
    for(unsigned int i = 0; i < counter_max.size(); i++)
    {
        counter_max[i] = image_size[i] - 1;
    }

    Scanner * p_scanner = 0;

    if(scanner_type == "scanline")
    {
        p_scanner = new SequentialScanner(counter_min, counter_max);
    }
    else if(scanner_type == "serpentine")
    {
        p_scanner = new SerpentineScanner(counter_min, counter_max);
    }
    else if((scanner_type == "random") || (scanner_type == "badrandom"))
    {
        p_scanner = new RandomScanner(counter_min, counter_max);
    }
    else if(scanner_type == "hilbert")
    {
        p_scanner = new HilbertScanner(counter_min, counter_max);
    }
    else
    {
        cerr << "unsupported scanner_type" << endl;
        return 1;
    }

    if(p_scanner == 0) throw Exception("empty scanner");
    Scanner & scanner = *p_scanner;

    // do the work
    Image output_image = input_image;
    
    vector<int> index(dimension);
    vector<int> neighbor(dimension);
    CoordinatePixel input_pixel, output_pixel;
    vector<CoordinatePixel> error_pixels;
    CoordinatePixel::Coordinate error_offset(dimension);
    scanner.Reset();
    do
    {
        scanner.GetCoordinate(input_pixel.coordinate);

        Utility::Convert(input_pixel.coordinate, index);
        if(! input_image.Get(index, input_pixel.value)) throw Exception("cannot get input_pixel");
        
        if(! diffuser.Diffuse(input_pixel, output_pixel, error_pixels)) throw Exception("cannot diffuse pixel");

        if(! output_image.Put(index, output_pixel.value)) throw Exception("cannot put output_pixel");

        if(!occupancy_map.Put(index, 1)) throw Exception("cannot update occupancy map");

        for(unsigned int i = 0; i < error_pixels.size(); i++)
        {
            const CoordinatePixel & error_pixel = error_pixels[i];

            if(error_pixel.coordinate.Dimension() != neighbor.size()) throw Exception("error_pixel.coordinate.Dimension() != neighbor.size()");

            if(! scanner.GetOffset(error_pixel.coordinate, error_offset)) throw Exception("cannot convert error offset");

            for(unsigned int j = 0; j < neighbor.size(); j++)
            {
                neighbor[j] = index[j] + error_offset[j];

#ifdef TOROIDAL_BOUNDARY
                neighbor[j] = ((neighbr[j]%image_size[j])+image_size[j])%image_size[j];
#endif
            }

            int inside = 1;
            for(unsigned int j = 0; j < neighbor.size(); j++)
            {
                if((neighbor[j] < 0) || (neighbor[j] >= image_size[j]))
                {
                    inside = 0; break;
                }
            }

            if(inside)
            {
                if(! input_image.Get(neighbor, input_pixel.value)) throw Exception("cannot get input_neighbor_pixel");
                for(int j = 0; j < input_pixel.value.Dimension(); j++)
                {
                    input_pixel.value[j] += error_pixel.value[j];
                }
                if(! input_image.Put(neighbor, input_pixel.value)) throw Exception("cannot put input_neighbor_pixel");
            }
        }
    }
    while(scanner.Next());

    // output result
    if(!WriteImage(output_image, maximum_value, color_dimension, output_file_name))
    {
        cerr << "cannot write to " << output_file_name << endl;
        return 1;
    }

    // clean up
    if(p_scanner)
    {
        delete p_scanner; p_scanner = 0;
    }

    if(p_thresholder)
    {
        delete p_thresholder; p_thresholder = 0;
    }

    if(p_super_thresholder)
    {
        delete p_super_thresholder; p_super_thresholder = 0;
    }

    if(p_distributor)
    {
        delete p_distributor; p_distributor = 0;
    }

    if(p_super_distributor)
    {
        delete p_super_distributor; p_super_distributor = 0;
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
