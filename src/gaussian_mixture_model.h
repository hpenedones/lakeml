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

#include "classifier.h"
#include "dataset.h"

#ifndef GMM_H_
#define GMM_H_


class GaussianMixtureModel: public Classifier
{
public:
    GaussianMixtureModel(int nGaussians, int maxIterations);
    void train(const Dataset & dataset, const vector<double> & weights);
    int  classify(const DataInstance & sample) const;
    double response(const DataInstance & sample) const;


private:

    //data
    vector< vector<double> > means;
    vector< vector<double> > diag_covs;

    vector< vector<double> > resps, temp;
    vector<double> weights, mass, log_sqrt_determinants;

    int ngaussians, iterations, max_iterations, dim, nsamples;
    double pi_const;

    //methods

    void run(const Dataset & dataset);

    void initialize(int dimension, int nsamples);
    void initialize_clusters_with_kmeans(const Dataset & dataset);

    void m_step(const Dataset & dataset);

    void e_step(const Dataset & dataset);

    void show_variables();

    int optimalNumOfGaussians(const Dataset & training, const Dataset & validation, int lim_inf, int lim_sup) const;

    double gaussian_exponent(const DataInstance & x, int gaussian_index) const;

    double exponent_form(const DataInstance & x, int g) const;

    double log_sqrt_determinant(int gaussian_index) const;

    double sample_log_likelihood(const DataInstance & sample) const;

    double datasetLogLikelihood(const Dataset &dataset) const;

};



#endif
