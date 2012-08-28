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




#ifndef EM_H_
#define EM_H_


class EM {
	
public:		
	
	EM( double ** data, int ns, int d, int ngauss, int max_iter, double delta_improv);
	
	~EM();
	
	
	void initialize();
	
	int  classify(double * sample);
	
	double dataset_log_likelihood(double **data, int rows);


	static int optimal_num_gaussians(double ** training, double ** validation,
		int dim, int ntrainsamples , int nvalsamples, int lim_inf, int lim_sup);


	void m_step();
	
	void e_step();
	
	void show_variables();
	
	void run();
	
	
private:
	
	double gaussian_exponent(double * x, int gaussian_index);
	
	double exponent_form(double *x, int g);
	
	double log_sqrt_determinant(int gaussian_index);

	double sample_log_likelihood(double *sample);
	
	
	double ** dataset;
	double ** means;
	double ** diag_covs;
	double ** resps, ** temp; 
	double * weights, * mass, *log_sqrt_determinants;
	double * tmp_exponents;
	int ngaussians, nsamples, dim;
	int iterations, max_iterations;
	double pi_const;
	
};



#endif 
