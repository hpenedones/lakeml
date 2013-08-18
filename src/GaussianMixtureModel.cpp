/*
 *   Copyright 2008-2012 Hugo Penedones
 *
 *   This file is part of lakeml.
 *
 *   lakeml is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   lakeml is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with lakeml.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _USE_MATH_DEFINES

#include <GaussianMixtureModel.h>
#include <cmath>
#include <float.h>
#include <iostream>
#include <math_utils.h>
#include <cassert>
#include <Kmeans.h>
#include <algorithm>
using namespace std;


GaussianMixtureModel::GaussianMixtureModel(int ngauss, int max_iter)
{
	ngaussians = ngauss;
	max_iterations = max_iter;
}

	
void GaussianMixtureModel::initialize(int dim, int nsamples)
	{

		pi_const = 0.5*dim*log(M_PI);
		iterations = 0;

		means.reserve(ngaussians);
		for (int i = 0; i < ngaussians; i++)
			means[i].reserve(dim);

		diag_covs.reserve(ngaussians);
		for (int i = 0; i < ngaussians; i++)
			diag_covs[i].reserve(dim);

		weights.reserve(ngaussians);

		resps.reserve(nsamples);
		for (int i = 0; i < nsamples; i++)
			resps[i].reserve(ngaussians);

		temp.reserve(nsamples);
		for (int i = 0; i < nsamples; i++)
			temp[i].reserve(ngaussians);

		log_sqrt_determinants.reserve(ngaussians);
		mass.reserve(ngaussians);		
	}

void GaussianMixtureModel::initialize_clusters_with_kmeans(const Dataset & dataset)
{
	// data   has size (nsamples * dim);

	Kmeans kmeans(dataset, ngaussians);
	kmeans.run(100, 0.1);

	for (int g = 0; g < ngaussians; ++g)
	{
		weights[g] = 1.0 / ngaussians;

		for (int d = 0; d < dim; ++d)
		{
			means[g][d] = kmeans.cluster_centers[g][d];
			/*
			\todo initialize covariances matrices with sample variance from kmeans
			*/
			diag_covs[g][d] = 400;
		}
	}

	//	pre-compute logarithms of square-root of determinants of the covariance-matrices
	for (int g = 0; g < ngaussians; ++g)
		log_sqrt_determinants[g] = log_sqrt_determinant(g);
}





double GaussianMixtureModel::gaussian_exponent(const DataInstance & x, int gaussian_index) const
	{
	double exponent = 0.0;
	
	for(int d = 0; d < dim; d++){
	//	assert(diag_covs[gaussian_index][d] > 0.001);
		double diff =  x[d] - means[gaussian_index][d];
		exponent += diff*(diff / diag_covs[gaussian_index][d]);
	}
	
	return exponent;
	}
	
double GaussianMixtureModel::exponent_form(const DataInstance & x, int g) const
	{
		return -0.5*gaussian_exponent(x, g) + log(weights[g]) - log_sqrt_determinants[g] -  pi_const;
	}

double GaussianMixtureModel::log_sqrt_determinant(int gaussian_index) const
	{
		double log_sqrt_det = 0.0;
		
		for(int d = 0; d < dim; ++d)
			log_sqrt_det += 0.5*log(diag_covs[gaussian_index][d]) ; 
		
		return log_sqrt_det;
	}


double GaussianMixtureModel::response(const DataInstance & sample) const
{
	return sample_log_likelihood(sample);
}

double GaussianMixtureModel::sample_log_likelihood(const DataInstance & sample) const
	{
		vector<double> tmp_exponents(ngaussians);

		for(int g = 0; g < ngaussians; ++g)
			tmp_exponents[g] = exponent_form(sample, g);
				
		double max_exponent = *std::max_element(tmp_exponents.begin(), tmp_exponents.end());
		assert(!isnan(max_exponent));
		
		double sum_exp = 0.0;
		for(int g = 0; g < ngaussians; ++g)
			sum_exp += exp(tmp_exponents[g] - max_exponent);
		
		assert(!isnan(sum_exp));

		double log_likelihood = max_exponent + log(sum_exp);  
	
		assert(!isnan(log_likelihood));
		return log_likelihood;
		
	}
	
	
	int  GaussianMixtureModel::classify(const DataInstance & sample) const
	{
		vector<double> tmp_exponents(ngaussians);

		for(int g = 0; g < ngaussians; ++g)
			tmp_exponents[g] = exponent_form(sample, g);
	
		int max_ind = std::max_element(tmp_exponents.begin(), tmp_exponents.end()) - tmp_exponents.begin();
		return max_ind;
	}
	
	double GaussianMixtureModel::datasetLogLikelihood(const Dataset &data) const
	{
			
		double acc = 0.0;
		for(int i = 0; i < data.size(); ++i)
		{
			acc += sample_log_likelihood(data[i]);
		}
		return acc;
	}


	int GaussianMixtureModel::optimalNumOfGaussians(const Dataset & training, const Dataset & validation,
	   								  int lim_inf, int lim_sup) const
	{
		double best_log_likelihood = -DBL_MAX;
		int optim_N = 0;

		for(int i = lim_inf; i <= lim_sup; ++i)
		{
			GaussianMixtureModel gmm(i, 100);

			vector<double> weights(training.size());
			std::fill(weights.begin(), weights.end(), 1.0);
			gmm.train(training, weights);

			double log_likelihood = gmm.datasetLogLikelihood(validation);
			double train_log_likelihood = gmm.datasetLogLikelihood(training);

			if ( log_likelihood > best_log_likelihood )
				{
					best_log_likelihood = log_likelihood;
					optim_N = i;
				}
		}

		assert(optim_N > 0);
		
		return optim_N;
	}



	void GaussianMixtureModel::m_step(const Dataset & dataset)
	{
		// compute "mass" of each gaussian
		double total_mass = 0.0;
		for(int g = 0; g < ngaussians; ++g)
		{
			mass[g] = 0.0;
			for(int s = 0; s < nsamples; ++s)
				mass[g]+= resps[s][g];
			total_mass += mass[g];
		}
		
		// update means
		for(int g=0; g<ngaussians; g++) 
			{
			for(int d = 0; d < dim; d++)
				{
				means[g][d] = 0.0;
				for(int s = 0; s < nsamples; ++s)
					means[g][d] += resps[s][g] * dataset[s][d];
				
				assert(mass[g] > 0.0);
				means[g][d] /= mass[g];
				}
			}
			
		// update diagonal covariance matrices
		for(int g=0; g<ngaussians; g++)
			for(int d = 0; d < dim; d++)
				{
				diag_covs[g][d] = 0.0;
				for(int s = 0; s < nsamples; ++s)
					{
						double diff = dataset[s][d] - means[g][d];
						diag_covs[g][d] += resps[s][g] * diff * diff;
					}
		//		assert(mass[g] > 0.001);
				diag_covs[g][d] /= mass[g];
				
				if  (diag_covs[g][d] < 1.0)
					diag_covs[g][d] = 1.0; // hack to avoid singularity problems
				
				}
		
			//	assert(total_mass > 0.001);
		// update weights (mixing coefficients) 
		for(int g = 0; g < ngaussians; ++g)
			weights[g] = mass[g]/total_mass;


		//	pre-compute logarithms of square-root of determinants of the covariance-matrices
		for(int g = 0; g < ngaussians; ++g)
			log_sqrt_determinants[g] = log_sqrt_determinant(g);
	    
	}
	
	void GaussianMixtureModel::e_step(const Dataset & dataset)
	{
		vector<double> tmp_exponents(ngaussians);
		for(int s = 0; s < nsamples; ++s)
		{	
			for(int g = 0; g < ngaussians; ++g)
				tmp_exponents[g] = -0.5*gaussian_exponent(dataset[s], g) + log(weights[g]) - log_sqrt_determinants[g] ;
			
			double max_exponent = *std::max_element(tmp_exponents.begin(), tmp_exponents.end());
			
			for(int g = 0; g < ngaussians; ++g)
				resps[s][g] = exp(tmp_exponents[g] - max_exponent);

			normalize(resps[s]);
		}

	}
	 
	void GaussianMixtureModel::show_variables()
	{
		
		for(int g = 0; g < ngaussians; ++g)
		{
			cout << "Gaussian " << g << ":" << endl;
			for(int d = 0; d < dim; ++d)
			{
			cout << means[g][d] << "  " << diag_covs[g][d] << " ";
			}	
			cout << endl;
		}
		cout << endl;
		
	}
	
	void GaussianMixtureModel::run(const Dataset & dataset)
	{
		// C++ style for -DBL_MAX
		double prev_log_likelihood = - numeric_limits<double>::max();
		double cur_log_likelihood = - numeric_limits<double>::max();
	

		do
		{
			e_step(dataset);
			m_step(dataset);

			prev_log_likelihood = cur_log_likelihood;
			cur_log_likelihood = datasetLogLikelihood(dataset);

		}
		while( iterations++ < max_iterations && cur_log_likelihood > prev_log_likelihood + 0.01);
		
	}

	
	void GaussianMixtureModel::train(const Dataset & dataset, const vector<double> & weights)
	{
		assert(dataset.size > 0);
		initialize(dataset[0].size(), dataset.size());
		initialize_clusters_with_kmeans(dataset);

		run(dataset);

	}


