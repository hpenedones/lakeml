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




#ifndef FEATEXTRCT
#define FEATEXTRCT

#include <DataInstance.h>

#ifdef USE_IS_ABSTRACT
#include <boost/serialization/is_abstract.hpp>
#else
#include <boost/serialization/assume_abstract.hpp>
#endif

#include <boost/serialization/export.hpp>



class FeatureExtractor {
	

public:
	
	virtual double getFeatureVal(const DataInstance * data_instance) const = 0;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	}
	
};

#ifdef USE_IS_ABSTRACT
BOOST_IS_ABSTRACT(FeatureExtractor)
#else
BOOST_SERIALIZATION_ASSUME_ABSTRACT(FeatureExtractor)
#endif

#endif


