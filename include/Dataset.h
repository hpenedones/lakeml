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

#include <set>
#include <vector>

#ifndef DATASET
#define DATASET

using namespace std;

class DataInstance {

public:

	DataInstance(int dimension) : data(dimension)
	{

	}

	double operator [](size_t feature_index) const
	{
		return data[feature_index];
	}

	size_t size() const
	{
		return data.size();
	}

protected:
	vector<double> data;
};


class Dataset {
	
public:	
	
	size_t size() const
	{
		return data.size();
	}
	
	const DataInstance & operator [](unsigned int sample_index) const 
	{
		return data[sample_index];
	}

	void add(DataInstance & sample, int label)
	{
		data.push_back(sample);
		labels.push_back(label);
	}

	int getLabelAt(size_t sample_index) const
	{
		return labels[sample_index];
	}


protected:
	
	vector< const DataInstance > data;
	vector< int > labels;

};

#endif


