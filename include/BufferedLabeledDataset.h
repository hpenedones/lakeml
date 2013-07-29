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
#include <DataSampler.h>
#include <DataInstanceLoader.h>
#include <LabeledDataset.h>
#include <WeightedSampler.h>

#ifndef BUFFLABDTS
#define BUFFLABDTS


// A buffered labeled dataset is representation of a dataset that might not fit totally in main memory
// It keeps an index of data loaders that allow to fetch data from wherever it is stored and it only keeps
// a small buffer (active set) loaded in memory. 
class BufferedLabeledDataset : public LabeledDataset
{
	
public:
	
	BufferedLabeledDataset(set<int> allowed_labels);
	~BufferedLabeledDataset();
	
	virtual const DataInstance * getDataInstanceAt(unsigned int pos) const; 
	virtual int getLabelAt(unsigned int index) const;
	virtual size_t size() const;


	void setNumPositivesInActiveSet(int npos);
	int getNumPositivesInActiveSet();

	void setNumNegativesInActiveSet(int nneg);
	int getNumNegativesInActiveSet();
	
	size_t getActiveSetSize() const;
	
	const LabeledDataset * getActiveDataset() const;
	
	void resampleActiveDataset(const vector<double> & sampling_weights, vector<double> & sampled_frequencies);
		
	void addDataLoader(DataInstanceLoader * data_loader, int label);
	

 
	// more low-level methods (move to private)

	void emptyBuffer();

	void loadToBuffer(vector<int> &selected_indexes);

	
private: 
	
	LabeledDataset active_set;
	WeightedSampler weighted_sampler;
	
	int npos_in_active_set, nneg_in_active_set;
	
	vector<DataInstanceLoader *> external_data_loaders;
	vector<int> full_dataset_labels;
//	vector<double> data_weights;
};

#endif

