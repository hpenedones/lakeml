#include <cassert>
#include <math_utils.h>

double max(double * array, size_t length)
{
	assert(length > 0);
	double max = array[0];
	
	for(size_t i = 1; i < length; ++i)
	{
		if(array[i] > max)
			max = array[i];
	}	
	return max;
}

double max_ind(double * array, size_t length)
{
	assert(length > 0);
	double max = array[0];
	int ind_max = 0;
	
	for(size_t i = 1; i < length; ++i)
	{
		if(array[i] > max)
			{
				max = array[i];
				ind_max = i;
			}
	}	
	return ind_max;
}

double max_in_col(double ** matrix, int nrows, int col)
{
	double max = matrix[0][col];
	
	for(int i = 1; i < nrows; ++i)
	{
		if(matrix[i][col] > max)
			max = matrix[i][col];
	}
	
	return max;
}

double min_in_col(double ** matrix, int nrows, int col)
{
	double min = matrix[0][col];
	
	for(int i = 1; i < nrows; ++i)
	{
		if(matrix[i][col] < min)
			min = matrix[i][col];
	}
	
	return min;
}

double min(double * array, size_t length)
{
	assert(length > 0);
	double min = array[0];
	
	for(size_t i = 1; i < length; ++i)
	{
		if(array[i] < min)
			min = array[i];
	}	
	return min;
}

int min_ind(double * array, size_t length)
{
	assert(length > 0);
	double min = array[0];
	int ind_min = 0;
	for(size_t i = 1; i < length; ++i)
	{
		if(array[i] < min)
			{
				min = array[i];
				ind_min = i;
			}
	}	
	return ind_min;
}

double sum(double * array, size_t length)
{
	double acc = 0.0;
	for(size_t i = 0; i < length; ++i)
	{
		acc += array[i];
	}
	return acc;
}

void normalize(double *array, size_t length)
{
	
	double total = sum(array, length);
	
	for(size_t i = 0; i < length; ++i)
	{
		array[i] /= total;
	}
	
}

int sign(double val)
{
	return 	(val >=0 ? 1 : -1);
}

int max(int a, int b)
{
	return (a > b ? a : b);
}

int min(int a, int b)
{
	return (a < b ? a : b);
}


double mean(const vector<double> & data, const vector<double> & weights)
{
	double sum = 0.0;
	double mass = 0.0;
	
	for(size_t i = 0; i < data.size(); i++)
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
	
	for(size_t i = 0; i < data.size(); i++)
	{
		sum_sqr_err += weights[i] * (data[i]-mean)*(data[i]-mean);
		mass += weights[i];
		
	}
	
	return sum_sqr_err / mass;	
	
}
