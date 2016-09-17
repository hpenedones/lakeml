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

#include "Dataset.h"

#ifndef KMEANS_H_
#define KMEANS_H_

class  Kmeans {

	friend class GaussianMixtureModel;

public:

	Kmeans(const Dataset & dataset, int nclusters);
	~Kmeans();

	int run(int max_iterations, float min_delta_improv);
	int getClosestClusterLabel(const DataInstance & x) const;

private:

	void initialize();
	void computeCenters();
	void updateAssignments();
	double computeError();
	void oneStep();

	double l2norm(const DataInstance & x, const vector<double> & y) const;

	vector<int> cluster_labels, counters;
	Dataset dataset;
	vector< vector<double> > cluster_centers;
	int nclusters;
	int nsamples, dim;
	int iterations;
	double prev_error, error;

};

#endif
