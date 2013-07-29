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




#include <assert.h>
#include <vector>
#include <iostream>
#include <LabeledDataset.h>

#ifdef USE_IS_ABSTRACT
#include <boost/serialization/is_abstract.hpp>
#else
#include <boost/serialization/assume_abstract.hpp>
#endif

//#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>

#ifndef CLASSIFIER
#define CLASSIFIER




class Classifier {
	

public:

//	Classifier();
//	virtual ~Classifier() = 0;
	
	
	virtual void   train(const LabeledDataset* training_dataset, vector<double> &weights) = 0;
	virtual double response(const DataInstance * data_instance) const = 0;
	virtual int	   classify(const DataInstance * data_instance) const = 0;
	
	vector<double> responses(const Dataset* dataset) const;
	vector<int>	   classifications(const Dataset* dataset) const;
	
	double classificationError(const LabeledDataset * dataset) const;
	double classificationError(const LabeledDataset * dataset, vector<double> &weights) const;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	}
	
protected:	
		

	
};

#ifdef USE_IS_ABSTRACT
BOOST_IS_ABSTRACT(Classifier)
#else
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Classifier)
#endif

#endif



