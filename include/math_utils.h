#include <vector>

using namespace std;

#ifndef MATH_UTILS_H_
#define MATH_UTILS_H_


double max(double * array, size_t length);

double max_ind(double * array, size_t length);

double max_in_col(double ** matrix, int nrows, int col);

double min_in_col(double ** matrix, int nrows, int col);

double min(double * array, size_t length);

int min_ind(double * array, size_t length);

double sum(double * array, size_t length);

void normalize(double *array, size_t length);

int sign(double val);

int max(int a, int b);

int min(int a, int b);

double mean(const vector<double> & data, const vector<double> & weights);

double variance(const vector<double> & data, const vector<double> & weights, double mean);


#endif 
