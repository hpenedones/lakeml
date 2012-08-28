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



#include <Classifier.h>
#include <vector>
#include <ClassifierFactory.h>
#include <boost/serialization/vector.hpp>
#include <iostream>

using namespace std;


#ifndef NAIVEBAYESCLASSIFIER_H_
#define NAIVEBAYESCLASSIFIER_H_


class NaiveBayesClassifier : public Classifier
{
	friend class boost::serialization::access;
	
public:
	NaiveBayesClassifier ();
	NaiveBayesClassifier(const ClassifierFactory* classifier_factory, int numweak_learners); 
	
	~NaiveBayesClassifier ();
	
	// declared virtual in Classifier
	void   train(const LabeledDataset* training_dataset, vector<double> &weights);
	double response(const DataInstance * data_instance) const;
	int	   classify(const DataInstance * data_instance) const;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Classifier );
		ar & BOOST_SERIALIZATION_NVP( decision_threshold );
		ar & BOOST_SERIALIZATION_NVP( weak_learners );

	}

private:
	
	const ClassifierFactory * classifier_factory;
	int learners_to_add;



	double decision_threshold;
	vector<Classifier *> weak_learners;

	
};




#endif 
