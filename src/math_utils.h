#include <vector>

#ifndef MATH_UTILS_H_
#define MATH_UTILS_H_

int sign(double val);

void normalize(std::vector<double> &v);

double sum(std::vector<double> & v);

double mean(const std::vector<double> & data, const std::vector<double> & weights);

double variance(const std::vector<double> & data, const std::vector<double> & weights, double mean);

#endif
