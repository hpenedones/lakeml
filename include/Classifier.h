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

#include <cassert>
#include <vector>
#include <Dataset.h>

#ifndef CLASSIFIER
#define CLASSIFIER

class Classifier 
{

public:

//	Classifier();
//	virtual ~Classifier() = 0;
		
	virtual void   train(const Dataset & training_dataset, vector<double> &weights) = 0;
	virtual double response(const DataInstance & data_instance) const = 0;
	virtual int	   classify(const DataInstance &  data_instance) const = 0;
	
	vector<double> Classifier::response(const Dataset & dataset) const{

		vector<double> resp;

		for (unsigned int i = 0; i < dataset.size(); i++)
			resp.push_back(response(dataset[i]));

		return resp;
	}

	vector<int> Classifier::classify(const Dataset & dataset) const{

		vector<int> classes;

		for (unsigned int i = 0; i < dataset.size(); i++)
			classes.push_back(classify(dataset[i]));

		return classes;
	}

		
};

#endif



