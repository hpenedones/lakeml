#include <cassert>
#include <algorithm>
#include <vector>
#include <numeric>

#include "math_utils.h"


int sign(double val)
{
	return 	(val >= 0 ? 1 : -1);
}

double sum(vector<double> &v)
{
	return std::accumulate(v.begin(), v.end(), 0);
}

void normalize(vector<double> &v)
{
	double s = sum(v);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		v[i] /= s;
	}
}


double mean(const vector<double> & data, const vector<double> & weights)
{
	double sum = 0.0;
	double mass = 0.0;

	for (size_t i = 0; i < data.size(); i++)
	{
		sum += data[i] * weights[i];
		mass += weights[i];
	}

	return sum / mass;
}

double variance(const vector<double> & data, const vector<double> & weights, double mean)
{
	double sum_sqr_err = 0.0;
	double mass = 0.0;

	for (size_t i = 0; i < data.size(); i++)
	{
		sum_sqr_err += weights[i] * (data[i] - mean) * (data[i] - mean);
		mass += weights[i];

	}

	return sum_sqr_err / mass;
}
