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



#include <UnlabeledDataset.h>
#include <cassert>

void UnlabeledDataset::addDataInstance( const DataInstance * data_instance){
	
	if (data.size() > 0)
    {
		assert(data[0]->isCompatible(data_instance));
    } 
	data.push_back(data_instance);
	
}

void UnlabeledDataset::removeDataInstanceAt(unsigned int pos)
{
	assert(pos < data.size() && pos > 0);
	data.erase(data.begin() + pos, data.begin() + pos + 1);
}

void UnlabeledDataset::clear() { 
	
	data.clear();
}

const DataInstance * UnlabeledDataset::getDataInstanceAt(unsigned int pos) const 
{
	assert(pos < data.size() && pos > 0);
	
	return data[pos];
}

size_t UnlabeledDataset::size() const 
{
	return data.size();
}

