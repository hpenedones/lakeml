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



#include <Dataset.h>
#include <DataInstance.h>

#ifndef LABELEDDATASET_H_
#define LABELEDDATASET_H_


class LabeledDataset: public Dataset {
	
public:

	LabeledDataset(const set<int> allowed_labels);
	virtual ~LabeledDataset();
	virtual void addDataInstance( const DataInstance * data_instance, int label);
	virtual const DataInstance * getDataInstanceAt(unsigned int pos) const; 
 	
	virtual void removeDataInstanceAt(unsigned int pos);
	virtual int getLabelAt(unsigned int pos) const;
	virtual void clear();
	virtual size_t size() const;

	int numDiffLabels() const;
	bool labelIsAllowed(int label) const;

private:

	set<int> allowed_labels;
	vector<int> labels;
	
};



#endif 