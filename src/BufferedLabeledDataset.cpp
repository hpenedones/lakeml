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



#include <iostream>
#include <BufferedLabeledDataset.h>


BufferedLabeledDataset::BufferedLabeledDataset(set<int> allowed_labels): 
LabeledDataset(allowed_labels), active_set(allowed_labels), weighted_sampler()
{

}

BufferedLabeledDataset::~BufferedLabeledDataset()
{
	
}

const LabeledDataset * BufferedLabeledDataset::getActiveDataset() const 
{
	return &active_set;
}

void BufferedLabeledDataset::setNumPositivesInActiveSet(int npos)
{
	npos_in_active_set = npos;	
}

int BufferedLabeledDataset::getNumPositivesInActiveSet()
{
	return npos_in_active_set;
}

size_t BufferedLabeledDataset::getActiveSetSize() const
{
	return npos_in_active_set + nneg_in_active_set;
}

void BufferedLabeledDataset::setNumNegativesInActiveSet(int nneg)
{
	nneg_in_active_set = nneg;
}

int BufferedLabeledDataset::getNumNegativesInActiveSet()
{
	return nneg_in_active_set;
}

void BufferedLabeledDataset::resampleActiveDataset(const vector<double> & sampling_weights, vector<double> & sampled_frequencies)
{
	assert(sampling_weights.size() == full_dataset_labels.size());
	assert((int) sampled_frequencies.size() == npos_in_active_set + nneg_in_active_set);
	
	active_set.clear();		
		
	vector<double> positive_samples_weights(sampling_weights.size(), 0.0);
	vector<double> negative_samples_weights(sampling_weights.size(), 0.0);
	
	for(unsigned int i = 0; i < sampling_weights.size(); i++)
	{
		if( getLabelAt(i) == 1 )
			positive_samples_weights[i] = sampling_weights[i];
		else
			negative_samples_weights[i] = sampling_weights[i];
	}
	
	vector<double> freq_pos(npos_in_active_set);
	vector<double> freq_neg(nneg_in_active_set);
	
	vector<int> selec_pos(npos_in_active_set);
	vector<int> selec_neg(nneg_in_active_set);
	
	
	double total_weight_pos = weighted_sampler.sample(positive_samples_weights, npos_in_active_set, selec_pos, freq_pos);
	double total_weight_neg = weighted_sampler.sample(negative_samples_weights, nneg_in_active_set, selec_neg, freq_neg);	
		
	double total_freq_neg = 0.0;	
	for(unsigned int i = 0; i < freq_neg.size(); i++)
	{
		total_freq_neg += freq_neg[i];
	}	
	
	double total_freq_pos = 0.0;	
	for(unsigned int i = 0; i < freq_pos.size(); i++)
	{
		total_freq_pos += freq_pos[i];
	}	
		
		
	// order is important here
	loadToBuffer(selec_pos);
	loadToBuffer(selec_neg);
	
	double mass = 0.0;
	for(int i = 0; i < (int) sampled_frequencies.size(); i++)
	{
		if(i < npos_in_active_set)
			sampled_frequencies[i] = total_weight_pos * freq_pos[i];
		else
			sampled_frequencies[i] = total_weight_neg * freq_neg[i-npos_in_active_set];
	
		mass += sampled_frequencies[i];
	}
	
	assert((int) active_set.size() == npos_in_active_set + nneg_in_active_set);
	
}



void BufferedLabeledDataset::emptyBuffer()
{
	active_set.clear();		
	assert(active_set.size() == 0);
}

void BufferedLabeledDataset::loadToBuffer(vector<int> &selected_indexes) {
		
	for(unsigned int i=0; i<selected_indexes.size(); i++)
	{
		int index = selected_indexes[i];

		const DataInstance * data_inst = external_data_loaders[index]->load();
		
		assert(full_dataset_labels[selected_indexes[0]] == full_dataset_labels[index] );
				
		active_set.addDataInstance(data_inst, full_dataset_labels[index]);
	}
}


void BufferedLabeledDataset::addDataLoader( DataInstanceLoader * data_loader, int label)
{
	full_dataset_labels.push_back(label);
	external_data_loaders.push_back(data_loader);
}

const DataInstance * BufferedLabeledDataset::getDataInstanceAt(unsigned int pos) const
{
	assert(pos >=0 && pos < (int) external_data_loaders.size());
	
	return external_data_loaders[pos]->load();
	
}

int BufferedLabeledDataset::getLabelAt(unsigned int index) const
{
	assert(index >=0 && index< (int)full_dataset_labels.size());
	
	return full_dataset_labels[index];
}

size_t BufferedLabeledDataset::size() const
{
	return full_dataset_labels.size();
}




