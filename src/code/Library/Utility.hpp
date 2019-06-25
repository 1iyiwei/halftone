/*
  Utility.hpp

  Li-Yi Wei
  11/19/2008

*/

#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <vector>
#include <string>
using namespace std;

#include "Sample.hpp"
#include "Pixel.hpp"
#include "FrameBuffer.hpp"
#include "Image.hpp"

class Utility
{
public:
    // squared distance between two samples
    static float Distance2(const Sample & s1, const Sample & s2);
    // specifying domain size for toroidal boundary condition
    static float Distance2(const Sample & s1, const Sample & s2, const vector<float> & domain_size);

    // pixel related arithmetics
    static float Distance2(const Pixel & p1, const Pixel & p2);
    static Pixel Diff(const Pixel & p1, const Pixel & p2);
    static Pixel Multiply(const Pixel & p1, const PixelType & c);

    // conversion routines between FrameBuffer and Image
    static int Convert(const FrameBuffer::P2 & input, Pixel & output);
    static int Convert(const Pixel & input, FrameBuffer::P2 & output);
    static int Convert(const Array<FrameBuffer::P2> & input, Image & output);
    static int Convert(const Image & input, Array<FrameBuffer::P2> & output);

    static int Convert(const FrameBuffer::P3 & input, Pixel & output);
    static int Convert(const Pixel & input, FrameBuffer::P3 & output);
    static int Convert(const Array<FrameBuffer::P3> & input, Image & output);
    static int Convert(const Image & input, Array<FrameBuffer::P3> & output);
    
    // other conversion routines
    static void Convert(const vector<int> & input, CoordinateT<int> & output);
    static void Convert(const CoordinateT<int> & output, vector<int> & input);

    static void Convert(const vector<int> & input, Pixel & output);

    // sample IO
    static int ReadSamples(const int dimension, const string & input_file_name, vector<Sample> & samples);

    static int WriteSamples(const vector<const Sample *> & samples, const string & output_file_name);

    enum SampleMode {SAMPLE_ASCII, SAMPLE_BINARY};

    static int ReadSamples(const SampleMode & mode, const int dimension, const string & input_file_name, vector<Sample> & samples);

    static int WriteSamples(const SampleMode & mode, const vector<const Sample *> & samples, const string & output_file_name);

    static int NumBits(const int value);
    static float UnsignedLongToNormalizedFloat(const unsigned long input);

    // handle argc/argv
    static vector<float> GetVector(int argc, char ** argv, const int dimension, int & argCtr);
};

#endif
