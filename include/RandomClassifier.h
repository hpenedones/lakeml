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
#include <Pose.h>
#include <DataInstance.h>
#include <LabeledDataset.h>

#include <vector>


using namespace std;


#ifndef RANDOMCLASSIFIER_H_
#define RANDOMCLASSIFIER_H_

class RandomClassifier : public Classifier
{
	
	public:
	RandomClassifier (double fraction_accept);
	~RandomClassifier ();

	void   train(const LabeledDataset* training_dataset, vector<double> &weights);
	double response(const DataInstance * data_instance) const;
	int	   classify(const DataInstance * data_instance) const;
	
	
private:

	double fraction_accept;
	
};



#endif 
