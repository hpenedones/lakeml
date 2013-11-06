#include <vector>

using namespace std;

#ifndef MATH_UTILS_H_
#define MATH_UTILS_H_

template<typename T> bool isfinite(T arg)
{
    return arg == arg && 
           arg != std::numeric_limits<T>::infinity() &&
           arg != -std::numeric_limits<T>::infinity();
}

int sign(double val);

void normalize(vector<double> &v);

double sum(vector<double> & v);

double mean(const vector<double> & data, const vector<double> & weights);

double variance(const vector<double> & data, const vector<double> & weights, double mean);


#endif 
