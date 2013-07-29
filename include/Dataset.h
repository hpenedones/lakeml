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
#include <DataInstance.h>

#ifndef DATASET
#define DATASET

using namespace std;


class Dataset {
	
public:	
	
	virtual size_t size() const =0;
	virtual const DataInstance * getDataInstanceAt(unsigned int pos) const =0;
	virtual void removeDataInstanceAt(unsigned int pos) = 0;
	virtual void clear() = 0;
	
protected:
	
	vector< const DataInstance * > data;
	
};


#endif


