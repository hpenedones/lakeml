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



#include <kmeans.h>
#include <iostream>
#include <cassert>
#include <math.h>
#include <cfloat>

Kmeans::Kmeans(double ** dataset, int nsamples, int dim, int nclusters)
{
	assert(nclusters > 0);
	assert(nsamples > nclusters);
	assert(dataset != NULL);
	
	this->dataset = dataset;
	this->nsamples = nsamples;
	this->dim = dim;
	this->nclusters = nclusters;
	this->iterations = 0;
	this->prev_error = DBL_MAX;
	
	cluster_centers = new double*[nclusters];
	for(int i = 0; i < nclusters; ++i)
		cluster_centers[i] = new double[dim];

	cluster_labels = new int[nsamples];
	counters = new int[nclusters];
	
}

Kmeans::~Kmeans()
{
	
	for(int i = 0; i < nclusters; ++i)
		delete[] cluster_centers[i];

	delete[] cluster_centers;
	delete[] cluster_labels;
	delete[] counters;
	
}

void Kmeans::initialize(){

	/* initialize random seed: */
	//	srand ( time(NULL) );
	int samples_per_cluster = nsamples / nclusters;
	
	for(int s=0; s<nsamples; s++)
		{
		cluster_labels[s] = s/samples_per_cluster;
		if (cluster_labels[s] > nclusters-1)
			cluster_labels[s] = nclusters-1;
		}
	// shuffle
	for(int s=0; s<nsamples; s++)
	{
		int new_pos = rand() % nsamples;
		int tmp = cluster_labels[s];
		cluster_labels[s] = cluster_labels[new_pos];
		cluster_labels[new_pos] = tmp;
	}
		computeCenters();
}


void Kmeans::computeCenters(){
	
	for(int i = 0; i < nclusters; i++)
		counters[i]=0;
		
	for (int s=0; s<nsamples ; s++) 
	{
		int cl = cluster_labels[s];
		counters[cl]++;
			
		for(int d=0; d<dim; d++)
			cluster_centers[cl][d] = cluster_centers[cl][d]*((counters[cl]-1.0)/counters[cl]) + 1.0*dataset[s][d]/counters[cl];	
	}

	
}

void Kmeans::updateAssignments(){

	for (int s=0; s<nsamples ; s++) {
		cluster_labels[s] = getClosestClusterLabel(dataset[s]);
	}
	
}


int Kmeans::getClosestClusterLabel(double * x)
{
	
	int ind=-1;
	double min = DBL_MAX, cur_dist;
	
	for(int i=0; i<nclusters; i++)
		{
			cur_dist = l2norm(x, cluster_centers[i]);  
			
			if (cur_dist < min) {
				min =cur_dist;
				ind =i;
			}
		}
		
	assert(ind >= 0);
		
	return ind;
	
}

double Kmeans::l2norm(double * x, double * y){
	
	double dist = 0.0;
	/*
		\todo make it more numerical stable (avoid overflow while taking the square)
	*/
	for(int i = 0; i < dim; ++i)
		dist += (x[i]-y[i])*(x[i]-y[i]);
	
	return sqrt(dist);
}

double Kmeans::computeError(){
	
	double error = 0.0;
	
	for(int s=0; s<nsamples; s++)
	{
		double dist = l2norm(dataset[s], cluster_centers[cluster_labels[s]]);
		
		error += dist;
	}
	
	return error;
}


void Kmeans::oneStep()
{
	prev_error = (iterations == 0) ? DBL_MAX : error;
	computeCenters();
	updateAssignments();
	error = computeError();

	iterations++;
}



int Kmeans::run(int max_iterations, float min_delta_improv){
	
	initialize();
	
	prev_error = DBL_MAX;
	error = computeError();
	
	iterations = 0;

	for ( ; (iterations < max_iterations) && (prev_error - error > min_delta_improv); iterations++)
	{
		// std::cout << "kmeans iteration # " << iterations << " current error = " << error << std::endl;
		
		computeCenters();
		updateAssignments();
		
		prev_error = error;
		error = computeError();
	}
	
	// std::cout << "kmeans iterations = " << iterations << " final error = " << error << std::endl;
	
	return iterations;
}


