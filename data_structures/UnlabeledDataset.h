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

#ifndef UNLABELEDDATASET_H_
#define UNLABELEDDATASET_H_


class UnlabeledDataset : public Dataset {
	
public:
	UnlabeledDataset();
	~UnlabeledDataset();
	const DataInstance * getDataInstanceAt(unsigned int pos) const;
	size_t size() const;
	void addDataInstance( const DataInstance * data_instance);
	void removeDataInstanceAt(unsigned int pos);
	void clear();
};



#endif 
