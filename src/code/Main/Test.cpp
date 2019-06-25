/*
  Test.cpp

  a holder for a bunch of different test routines

  Li-Yi Wei
  05/01/2009

*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <stdlib.h>
#include <math.h>

#include "HilbertCounter.hpp"
#include "Exception.hpp"

int Hilbert(int argc, char **argv)
{
    const int min_argc = 4;

    if(argc < min_argc)
    {
        cerr << "Usage: " << argv[0] << " dimension digit_min digit_max" << endl;
        return 1;
    }

    int arg_ctr = 0;
    const int dimension = atoi(argv[++arg_ctr]);
    const int digit_min = atoi(argv[++arg_ctr]);
    const int digit_max = atoi(argv[++arg_ctr]);

    HilbertCounter counter(dimension, digit_min, digit_max);

    vector<int> index(dimension);

    counter.Reset();
    do
    {
        counter.Get(index);

        for(unsigned int i = 0; i < index.size(); i++)
        {
            cout << index[i] << " ";
        }
        cout << endl;
    }
    while(counter.Next());

    // done
    return 1;
}

int Vector(int argc, char **argv)
{
    vector<int> foo;

    foo.insert(foo.begin(), 0);
    foo.insert(foo.end(), 1);
    foo.insert(foo.begin()+1, 2);

    for(unsigned int i = 0; i < foo.size(); i++)
    {
        cout << foo[i] << " ";
    }
    cout << endl;

    return 0;
}

int Main(int argc, char **argv)
{
    const int min_argc = 2;
    if(argc < min_argc)
    {
        cerr << "Usage: " << argv[0] << " test-name" << endl;
        return 1;
    }

    int arg_ctr = 0;
    const string test_name = argv[++arg_ctr];
    
    if(test_name == "hilbert")
    {
        return Hilbert(argc-arg_ctr, argv+arg_ctr);
    }
    else if(test_name == "vector")
    {
        return Vector(argc-arg_ctr, argv+arg_ctr);
    }
    else
    {
        cerr << "unknown test-name" << endl;
        return 1;
    }

    // should't reach here anyway
    throw Exception("shouldn't reach here!");
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
