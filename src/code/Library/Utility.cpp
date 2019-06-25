/*
  Utility.cpp

  Li-Yi Wei
  11/19/2008

*/

#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <math.h>

#include "Utility.hpp"
#include "Exception.hpp"

float Utility::Distance2(const Sample & s1, const Sample & s2)
{
    if(s1.coordinate.Dimension() != s2.coordinate.Dimension()) throw Exception("s1.coordinate.Dimension() != s2.coordinate.Dimension()");

    float distance = 0;

    for(int i = 0; i < s1.coordinate.Dimension(); i++)
    {
        const float diff = s1.coordinate[i] - s2.coordinate[i];
        distance += diff*diff;
    }

    return distance;
}

float Utility::Distance2(const Sample & s1, const Sample & s2, const vector<float> & domain_size)
{
    if(s1.coordinate.Dimension() != s2.coordinate.Dimension()) throw Exception("s1.coordinate.Dimension() != s2.coordinate.Dimension()");

    if(s1.coordinate.Dimension() != domain_size.size()) throw Exception("s1.coordinate.Dimension() != domain_size.size()");

    float distance = 0;

    for(int i = 0; i < s1.coordinate.Dimension(); i++)
    {
        const float diff = min(fabs(s1.coordinate[i] - s2.coordinate[i]), fabs(domain_size[i] - fabs(s1.coordinate[i] - s2.coordinate[i])));
        distance += diff*diff;
    }

    return distance;
}

float Utility::Distance2(const Pixel & p1, const Pixel & p2)
{
    if(p1.Dimension() != p2.Dimension()) throw Exception("p1.Dimension() != p2.Dimension()");

    float distance = 0;

    for(int i = 0; i < p1.Dimension(); i++)
    {
        const float diff = p1[i] - p2[i];
        distance += diff*diff;
    }

    return distance;
}

Pixel Utility::Diff(const Pixel & p1, const Pixel & p2)
{
    if(p1.Dimension() != p2.Dimension()) throw Exception("p1.Dimension() != p2.Dimension()");

    Pixel result(p1);

    for(int i = 0; i < result.Dimension(); i++)
    {
        result[i] = p1[i] - p2[i];
    }

    return result;
}

Pixel Utility::Multiply(const Pixel & p1, const PixelType & c)
{
    Pixel result(p1);

    for(int i = 0; i < result.Dimension(); i++)
    {
        result[i] = p1[i]*c;
    }

    return result;
}

int Utility::Convert(const FrameBuffer::P2 & input, Pixel & output)
{
    output = Pixel(1);
    output[0] = input;

    return 1;
}

int Utility::Convert(const Pixel & input, FrameBuffer::P2 & output)
{
    if(input.Dimension() == 1)
    {
        output = input[0];
        return 1;
    }
    else
    {
        return 0;
    }
}

int Utility::Convert(const Array<FrameBuffer::P2> & input, Image & output)
{
    vector<int> size;
    input.Size(size);
    output = Image(size);

    vector<FrameBuffer::P2> input_pixels;
    if(! input.Get(input_pixels)) return 0;

    vector<Pixel> output_pixels;
    if(! output.Get(output_pixels)) return 0;

    if(input_pixels.size() != output_pixels.size()) throw Exception("input_pixels.size() != output_pixels.size()"); // shouldn't happen

    for(unsigned int i = 0; i < output_pixels.size(); i++)
    {
        if(! Convert(input_pixels[i], output_pixels[i])) return 0;
    }

    if(! output.Put(output_pixels)) return 0;

    // done
    return 1;
}

int Utility::Convert(const Image & input, Array<FrameBuffer::P2> & output)
{
    vector<int> size;
    input.Size(size);
    output = Array<FrameBuffer::P2>(size);

    vector<Pixel> input_pixels;
    if(! input.Get(input_pixels)) return 0;

    vector<FrameBuffer::P2> output_pixels;
    if(! output.Get(output_pixels)) return 0;

    if(input_pixels.size() != output_pixels.size()) throw Exception("input_pixels.size() != output_pixels.size()"); // shouldn't happen

    for(unsigned int i = 0; i < output_pixels.size(); i++)
    {
        if(! Convert(input_pixels[i], output_pixels[i])) return 0;
    }

    if(! output.Put(output_pixels)) return 0;

    // done
    return 1;
}

int Utility::Convert(const FrameBuffer::P3 & input, Pixel & output)
{
    output = Pixel(3);
    output[0] = input.r;
    output[1] = input.g;
    output[2] = input.b;

    return 1;
}

int Utility::Convert(const Pixel & input, FrameBuffer::P3 & output)
{
    if(input.Dimension() == 3)
    {
        output.r = input[0];
        output.g = input[1];
        output.b = input[2];
        return 1;
    }
    else
    {
        return 0;
    }
}

int Utility::Convert(const Array<FrameBuffer::P3> & input, Image & output)
{
    vector<int> size;
    input.Size(size);
    output = Image(size);

    vector<FrameBuffer::P3> input_pixels;
    if(! input.Get(input_pixels)) return 0;

    vector<Pixel> output_pixels;
    if(! output.Get(output_pixels)) return 0;

    if(input_pixels.size() != output_pixels.size()) throw Exception("input_pixels.size() != output_pixels.size()"); // shouldn't happen

    for(unsigned int i = 0; i < output_pixels.size(); i++)
    {
        if(! Convert(input_pixels[i], output_pixels[i])) return 0;
    }

    if(! output.Put(output_pixels)) return 0;

    // done
    return 1;
}

int Utility::Convert(const Image & input, Array<FrameBuffer::P3> & output)
{
    vector<int> size;
    input.Size(size);
    output = Array<FrameBuffer::P3>(size);

    vector<Pixel> input_pixels;
    if(! input.Get(input_pixels)) return 0;

    vector<FrameBuffer::P3> output_pixels;
    if(! output.Get(output_pixels)) return 0;

    if(input_pixels.size() != output_pixels.size()) throw Exception("input_pixels.size() != output_pixels.size()"); // shouldn't happen

    for(unsigned int i = 0; i < output_pixels.size(); i++)
    {
        if(! Convert(input_pixels[i], output_pixels[i])) return 0;
    }

    if(! output.Put(output_pixels)) return 0;

    // done
    return 1;
}

void Utility::Convert(const vector<int> & input, CoordinateT<int> & output)
{
    output = CoordinateT<int>(input.size());

    for(int i = 0; i < output.Dimension(); i++)
    {
        output[i] = input[i];
    }
}

void Utility::Convert(const CoordinateT<int> & input, vector<int> & output)
{
    output = vector<int>(input.Dimension());

    for(unsigned int i = 0; i < output.size(); i++)
    {
        output[i] = input[i];
    }
}

void Utility::Convert(const vector<int> & input, Pixel & output)
{
    output = Pixel(input.size());

    for(unsigned int i = 0; i < output.Dimension(); i++)
    {
        output[i] = input[i];
    }
}

int Utility::ReadSamples(const int dimension, const string & input_file_name, vector<Sample> & samples)
{
    return ReadSamples(SAMPLE_ASCII, dimension, input_file_name, samples);
}

int Utility::WriteSamples(const vector<const Sample *> & samples, const string & output_file_name)
{
    return WriteSamples(SAMPLE_ASCII, samples, output_file_name);
}

int Utility::ReadSamples(const SampleMode & mode, const int dimension, const string & input_file_name, vector<Sample> & samples)
{
    ifstream input(input_file_name.c_str());
    samples.clear();

    if(! input)
    {
        return 0;
    }
    else
    {
        Sample sample(dimension);
        
        while(input.good())
        {
            input >> sample.id;

            for(int i = 0; i < dimension; i++)
            {
                if(mode == SAMPLE_ASCII)
                {
                    input >> sample.coordinate[i];
                }
                else
                {
                    unsigned long foo;
                    float bar;
                    input >> foo;
                    bar = *(reinterpret_cast<float *>(&foo));
                    sample.coordinate[i] = bar;
                }
            }
            
            if(input.good())
            {
                samples.push_back(sample);
            }
        }
    }

    return 1;
}

int Utility::WriteSamples(const SampleMode & mode, const vector<const Sample *> & samples, const string & output_file_name)
{
    const int is_cout = (output_file_name == "cout");

    ofstream output(is_cout ? "" : output_file_name.c_str());

    if(!is_cout && !output)
    {
        return 0;
    }
    else if(mode == SAMPLE_ASCII)
    {
        for(unsigned int i = 0; i < samples.size(); i++)
        {
            if(is_cout)
                cout << samples[i]->id;
            else
                output << samples[i]->id;

            for(int j = 0; j < samples[i]->coordinate.Dimension(); j++)
            {
                if(is_cout)
                    cout << " " << samples[i]->coordinate[j];
                else
                    output << " " << samples[i]->coordinate[j];
            }

            if(is_cout)
                cout << endl;
            else
                output << endl;
        }
    }
    else
    {
        if(sizeof(unsigned long) != sizeof(float)) throw Exception("sizeof(unsigned long) != sizeof(float)");
        
        unsigned long foo = 0;
        float bar = 0;

        for(unsigned int i = 0; i < samples.size(); i++)
        {
            if(is_cout)
                cout << samples[i]->id;
            else
                output << samples[i]->id;

            for(int j = 0; j < samples[i]->coordinate.Dimension(); j++)
            {
                bar = samples[i]->coordinate[j];
                foo = *(reinterpret_cast<unsigned long *>(&bar));
             
                if(is_cout)
                    cout << " " << foo;
                else
                    output << " " << foo;
            }

            if(is_cout)
                cout << endl;
            else
                output << endl;
        }
    }

    return 1;
}

int Utility::NumBits(const int value)
{
    return static_cast<int>(ceil(log(value*1.0)/log(2.0)));
}

float Utility::UnsignedLongToNormalizedFloat(const unsigned long input)
{
    if(sizeof(float) != sizeof(unsigned long)) throw Exception("sizeof(float) != sizeof(unsigned long)");

    float output = 0.0;

    unsigned long & alias = *reinterpret_cast<unsigned long *>(&output);

    alias = 0x3f800000 | (input & 0x007fffff);
    output -= 1.0;

    return output;
}

vector<float> Utility::GetVector(int argc, char ** argv, const int dimension, int & argCtr)
{
    vector<float> result;
    while(((argCtr+1) < argc) && (result.size() < dimension))
    {
        result.push_back(atof(argv[++argCtr]));
    }

    if(result.size() != dimension)
    {
        throw Exception("result.size() != dimension");
    }

    return result;
}
