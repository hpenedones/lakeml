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




#ifndef THRLRN
#define THRLRN

#include <Classifier.h>
#include <FeatureExtractor.h>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>


/// Simple classifier that finds the best threshold to separate two classes based on a single feature
class ThresholdLearner : public Classifier
{

	friend class boost::serialization::access;

public:
	ThresholdLearner();
	ThresholdLearner( FeatureExtractor * feature_extractor);
	
	// inherited from Classifier
	void train(const LabeledDataset * training_dataset, vector<double> &data_weights);
	double response(const DataInstance * data_instance) const;
	int	   classify(const DataInstance * data_instance) const;

	template<class Archive>  void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Classifier );
		ar & BOOST_SERIALIZATION_NVP( optimal_threshold );
		ar & BOOST_SERIALIZATION_NVP( label_on_left );
		ar & BOOST_SERIALIZATION_NVP( feature_extractor);
		
	}
	
  private:
	

	FeatureExtractor * feature_extractor;
	double optimal_threshold;
	int label_on_left;

};



#endif


