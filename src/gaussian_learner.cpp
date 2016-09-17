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

#include <cmath>

#include "gaussian_learner.h"

GaussianLearner::GaussianLearner()
{
    pos_class_mean = 0;
    pos_class_var  = 1;
    neg_class_mean = 0;
    neg_class_var  = 1;
    log_resp_shift = 0;
}

GaussianLearner::GaussianLearner(unsigned int feature_index)
{
    this->feature_index = feature_index;
    pos_class_mean = 0;
    pos_class_var = 1;
    neg_class_mean = 0;
    neg_class_var = 1;
    log_resp_shift = 0;
}

GaussianLearner::~GaussianLearner()
{

}


void GaussianLearner::train(const Dataset & training_dataset, const vector<double> &data_weights)
{
    assert(training_dataset.size() > 0);
    assert(training_dataset.size() == data_weights.size());

    vector<double> pos_class_resp, neg_class_resp;

    // compute feature (real-valued number) for each data sample
    for (unsigned int i = 0; i < training_dataset.size(); i++)
    {
        double fval = training_dataset[i][feature_index];

        if (isfinite(fval) )  // discard samples where feature is N.A.
        {
            if ( training_dataset.getLabelAt(i) == 1)
                pos_class_resp.push_back(fval);
            else
                neg_class_resp.push_back(fval);
        }
    }

    if (pos_class_resp.size() <= 2 || neg_class_resp.size() <= 2 )
    {
        return; // can't learn! keep default useless values
    }

    // assert(pos_class_resp.size() >= 2);
    // assert(neg_class_resp.size() >= 2);

    pos_class_mean = mean(pos_class_resp, data_weights);
    pos_class_var  = variance(pos_class_resp, data_weights, pos_class_mean);

    neg_class_mean = mean(neg_class_resp, data_weights);
    neg_class_var  = variance(neg_class_resp, data_weights, neg_class_mean);

    log_resp_shift = log(sqrt(2 * M_PI * pos_class_var)) - log(sqrt(2 * M_PI * neg_class_var));
}

// return log probability

double GaussianLearner::log_probability_pos_class(double val) const
{
    return -0.5 * pow(val - pos_class_mean, 2) / pos_class_var - log(sqrt(2 * M_PI * pos_class_var));
}

double GaussianLearner::log_probability_neg_class(double val) const
{
    return -0.5 * pow(val - neg_class_mean, 2) / neg_class_var - log(sqrt(2 * M_PI * neg_class_var));
}

double GaussianLearner::response(const DataInstance & data_instance) const
{
    double val = data_instance[feature_index];

    if (!isfinite(val))
        return val;

    double result = -0.5 * ( (val - pos_class_mean) * (val - pos_class_mean) / pos_class_var -
                             (val - neg_class_mean) * (val - neg_class_mean) / neg_class_var     );

    return result;
}

int GaussianLearner::classify(const DataInstance & data_instance) const
{
    double fval = response(data_instance);

    if (!isfinite(fval))
        return 0;

    if ( fval > log_resp_shift)
        return 1;

    return -1;
}

