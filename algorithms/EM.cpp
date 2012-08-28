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



#include <EM.h>
#include <cmath>
#include <float.h>
#include <iostream>
#include <math_utils.h>
#include <cassert>
#include <kmeans.h>

using namespace std;

	
EM::EM( double ** data, int ns, int d, int ngauss, int max_iter, double delta_improv)
	{
		iterations = 0;
		max_iterations = max_iter;
		dataset = data;
		ngaussians = ngauss;
		dim = d;
		nsamples = ns;
		pi_const = 0.5*dim*log(M_PI);
		
		means     = new double* [ngaussians];
		for(int i=0; i<ngaussians; i++) means[i] = new double[dim];
		
		diag_covs = new double* [ngaussians];
		for(int i=0; i<ngaussians; i++) diag_covs[i] = new double[dim];
		
		weights   = new double[ngaussians];
			
		resps	  = new double* [nsamples];
		for(int i=0; i<nsamples; i++) resps[i] = new double[ngaussians];	


		temp	  = new double* [nsamples];
		for(int i=0; i<nsamples; i++) temp[i] = new double[ngaussians];	

		log_sqrt_determinants = new double[ngaussians];
		tmp_exponents = new double[ngaussians];
		mass      = new double[ngaussians];
		
		// data   has size (nsamples * dim);
	}
	
	EM::~EM()
	{
		for(int i=0; i<ngaussians; i++) delete[] means[i];
		delete[] means;
		
		for(int i=0; i<ngaussians; i++) delete[] diag_covs[i];
		delete[] diag_covs;
		
		for(int i=0; i<nsamples; i++) delete[] resps[i];	
		delete[] resps;
		
		delete[] weights;
		delete[] tmp_exponents;
		delete[] log_sqrt_determinants;
		delete[] mass;
		
	}
	
	
	void EM::initialize()
	{

		Kmeans * kmeans = new Kmeans(dataset, nsamples, dim, ngaussians);
		kmeans->run(100, 0.1);
		
		for(int g = 0; g < ngaussians; ++g)
		{
			weights[g] = 1.0/ngaussians;
			
			for(int d = 0; d < dim; ++d)
			{
				means[g][d] = kmeans->cluster_centers[g][d];
				/*
					\todo initialize covariances matrices with sample variance from kmeans
				*/
				diag_covs[g][d] = 400;
			}
		}
		
		delete kmeans;
		
		//	pre-compute logarithms of square-root of determinants of the covariance-matrices
		for(int g = 0; g < ngaussians; ++g)
			log_sqrt_determinants[g] = log_sqrt_determinant(g);
		
	}

	double EM::gaussian_exponent(double * x, int gaussian_index)
	{
	double exponent = 0.0;
	
	for(int d = 0; d < dim; d++){
	//	assert(diag_covs[gaussian_index][d] > 0.001);
		double diff =  x[d] - means[gaussian_index][d];
		exponent += diff*(diff / diag_covs[gaussian_index][d]);
	}
	
	return exponent;
	}
	
	double EM::exponent_form(double *x, int g)
	{
		return -0.5*gaussian_exponent(x, g) + log(weights[g]) - log_sqrt_determinants[g] -  pi_const;
	}

	double EM::log_sqrt_determinant(int gaussian_index)
	{
		double log_sqrt_det = 0.0;
		
		for(int d = 0; d < dim; ++d)
			log_sqrt_det += 0.5*log(diag_covs[gaussian_index][d]) ; 
		
		return log_sqrt_det;
	}

	//
	double EM::sample_log_likelihood(double *sample)
	{
		for(int g = 0; g < ngaussians; ++g)
			tmp_exponents[g] = exponent_form(sample, g);
				
		double max_exponent = max(tmp_exponents, ngaussians);
		assert(!isnan(max_exponent));
		
		double sum_exp = 0.0;
		for(int g = 0; g < ngaussians; ++g)
			sum_exp += exp(tmp_exponents[g] - max_exponent);
		
		if(isnan(sum_exp))
		{
			sum_exp = 0.0;
			for(int g = 0; g < ngaussians; ++g)
			{
				sum_exp += exp(tmp_exponents[g] - max_exponent);
				
			 cout <<  	sum_exp << " +=	exp( " << tmp_exponents[g] << " - " << max_exponent << ") = " << exp(tmp_exponents[g] - max_exponent) << endl;
			}
		exit(0);
		}

		double log_likelihood = max_exponent + log(sum_exp);  
	
		assert(!isnan(log_likelihood));
		return log_likelihood;
		
	}
	
	
	int  EM::classify(double * sample)
	{
		for(int g = 0; g < ngaussians; ++g)
			tmp_exponents[g] = exponent_form(sample, g);
		return max_ind(tmp_exponents, ngaussians);
	}
	
	double EM::dataset_log_likelihood(double **data, int rows)
	{
		//	pre-compute logarithms of square-root of determinants of the covariance-matrices
		for(int g = 0; g < ngaussians; ++g)
			log_sqrt_determinants[g] = log_sqrt_determinant(g);
			
		double acc = 0.0;
		for(int i = 0; i < rows; ++i)
		{
			acc += sample_log_likelihood(data[i]);
		}
		return acc;
	}


	int EM::optimal_num_gaussians(double ** training, double ** validation,
	   								 int dim, int ntrainsamples , int nvalsamples, int lim_inf, int lim_sup)
	{
		double best_log_likelihood = -DBL_MAX;
		int optim_N = 0;

		for(int i = lim_inf; i <= lim_sup; ++i)
		{
			EM * em_model = new EM ( training, ntrainsamples, dim, i, 100, 0.1);

			em_model->run();

			double log_likelihood = em_model->dataset_log_likelihood(validation, nvalsamples);
			double train_log_likelihood = em_model->dataset_log_likelihood(training, ntrainsamples);

			cout << i << " " << train_log_likelihood << " " << log_likelihood << endl; 

			if ( log_likelihood > best_log_likelihood )
				{
					best_log_likelihood = log_likelihood;
					optim_N = i;
				}
			delete em_model;
		}

		assert(optim_N > 0);
		
		return optim_N;
	}



	void EM::m_step()
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
	
	void EM::e_step()
	{

		for(int s = 0; s < nsamples; ++s)
		{	
			for(int g = 0; g < ngaussians; ++g)
				tmp_exponents[g] = -0.5*gaussian_exponent(dataset[s], g) + log(weights[g]) - log_sqrt_determinants[g] ;
			
			double max_exponent = max(tmp_exponents, ngaussians);
			
			for(int g = 0; g < ngaussians; ++g)
				resps[s][g] = exp(tmp_exponents[g] - max_exponent);

			normalize(resps[s], ngaussians);
		}

	}
	
	void EM::show_variables()
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
	
	void EM::run()
	{
		// C++ style for -DBL_MAX
		double prev_log_likelihood = - numeric_limits<double>::max();
		double cur_log_likelihood = - numeric_limits<double>::max();
	
		initialize();
		do
		{
			e_step();
			m_step();

			prev_log_likelihood = cur_log_likelihood;
			cur_log_likelihood = dataset_log_likelihood(dataset, nsamples);

		}
		while( iterations++ < max_iterations && cur_log_likelihood > prev_log_likelihood + 0.01);
		
	}

	


