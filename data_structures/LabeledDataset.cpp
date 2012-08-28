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



#include <LabeledDataset.h>
#include <cassert>


LabeledDataset::LabeledDataset(const set<int> labels_ok): allowed_labels(labels_ok), labels()
{
}

LabeledDataset::~LabeledDataset()
{
	
}

 void LabeledDataset::addDataInstance(  const DataInstance * data_instance, int label)
{
	assert( allowed_labels.count(label) == 1 );
	
	if (data.size() > 0)
    {
		assert(data_instance->isCompatible(data[0]));
    }
	
	data.push_back(data_instance);
	labels.push_back(label);
}

 int LabeledDataset::getLabelAt(unsigned int pos) const
{
	return labels[pos]; 
}

 void LabeledDataset::removeDataInstanceAt(unsigned int pos)
{
	assert(pos < data.size() && pos >= 0);
	data.erase(data.begin() + pos, data.begin() + pos + 1);
	labels.erase(labels.begin() + pos, labels.begin() + pos +1);
}

int LabeledDataset::numDiffLabels() const
{
	return allowed_labels.size();
}

bool LabeledDataset::labelIsAllowed(int label) const
{
	return (allowed_labels.find(label) != allowed_labels.end());
}

 void LabeledDataset::clear() 
{ 

	data.clear(); 
	labels.clear();

}

const DataInstance * LabeledDataset::getDataInstanceAt(unsigned int pos) const 
{
	assert(pos < data.size() && pos >= 0);
	
	return data[pos];
}

 size_t LabeledDataset::size() const 
{
	return data.size();
}


