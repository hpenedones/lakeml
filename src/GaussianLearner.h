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

#include <math.h>

#include "Classifier.h"
#include "math_utils.h"
#include "Dataset.h"

#ifndef GAUSSIANLEARNER_H_
#define GAUSSIANLEARNER_H_

class GaussianLearner: public Classifier
{

public:

	GaussianLearner();
	GaussianLearner(unsigned int feature_index);
	~GaussianLearner();

	void train(const Dataset & training_dataset, const vector<double> &data_weights);
	double response(const DataInstance & data_instance) const;
	int	   classify(const DataInstance & data_instance) const;

private:

	unsigned int feature_index;

	double log_probability_pos_class(double val) const;
	double log_probability_neg_class(double val) const;

	double pos_class_mean, pos_class_var;
	double neg_class_mean, neg_class_var;

	double log_resp_shift;
};



#endif
