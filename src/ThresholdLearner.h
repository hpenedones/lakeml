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

#ifndef THRLRN
#define THRLRN

#include "Classifier.h"

/// Simple classifier that finds the best threshold to separate two classes based on a single feature
class ThresholdLearner : public Classifier
{
public:
	ThresholdLearner();
	ThresholdLearner( unsigned int feature_index);

	// inherited from Classifier
	void train(const Dataset & training_dataset, const vector<double> &data_weights);
	double response(const DataInstance & data_instance) const;
	int	   classify(const DataInstance & data_instance) const;

private:

	unsigned int feature_index;
	double optimal_threshold;
	int label_on_left;

};

#endif


