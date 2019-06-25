/*
  Histogram.cpp

  count the pixel histogram of a given (halftoned) image

  Li-Yi Wei
  09/30/2009

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include <stdlib.h>
#include <math.h>

#include "Utility.hpp"
#include "FrameBuffer.hpp"
#include "Exception.hpp"

bool Equal(const FrameBuffer::P3 & pixel1, const FrameBuffer::P3 & pixel2)
{
    return ((pixel1.r == pixel2.r) && (pixel1.g == pixel2.g) && (pixel1.b == pixel2.b));
}

int Main(int argc, char **argv)
{
    // input arguments
    int min_argc = 2;

    if(argc < min_argc)
    {
        cerr << "Usage: " << argv[0] << " input_file_name" << endl;
        return 1;
    }

    int arg_ctr = 0;
    const string input_file_name = argv[++arg_ctr];

    // read input
    Array<FrameBuffer::P3> input;
    int maximum_value = 0;
    if(!FrameBuffer::ReadPPM(input_file_name.c_str(), input, maximum_value))
    {
        cerr << "cannot read ppm from " << input_file_name << endl;
        return 1;
    }

    // raw pixels
    vector<FrameBuffer::P3> pixels;
    if(! input.Get(pixels)) throw Exception("cannot get input pixels");

    // build palette
    vector<FrameBuffer::P3> palette;
    for(unsigned int i = 0; i < pixels.size(); i++)
    {
        int found = 0;
        for(unsigned int j = 0; j < palette.size(); j++)
        {
            if(Equal(pixels[i], palette[j])) found = 1;
        }

        if(!found)
        {
            palette.push_back(pixels[i]);
        }
    }

    // counting
    vector<unsigned int> count(palette.size(), 0);
    for(unsigned int i = 0; i < pixels.size(); i++)
    {
        int index = -1;
        for(unsigned int j = 0; j < palette.size(); j++)
        {
            if(Equal(pixels[i], palette[j]))
            {
                if(index < 0)
                {
                    index = j;
                }
                else
                {
                    throw Exception("duplicate palette entry");
                }
            }
        }

        if((index >= 0) && (index < count.size()))
        {
            count[index]++;
        }
        else
        {
            throw Exception("missing palette entry");
        }
    }
    
    // reporting
    cout << palette.size() << " colors" << endl;
    for(unsigned int i = 0; i < palette.size(); i++)
    {
        cout << "(" << palette[i].r << " " << palette[i].g << " " << palette[i].b << "): " << count[i]*1.0/pixels.size() << endl;
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
