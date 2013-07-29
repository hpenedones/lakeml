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

vector<double> Classifier::responses(const Dataset * dataset) const{
	
	vector<double> resp;
	
	for(unsigned int i=0; i<dataset->size(); i++)
		resp.push_back(response(dataset->getDataInstanceAt(i)));
	
	return resp;
}

vector<int> Classifier::classifications(const Dataset * dataset) const{
	
	vector<int> classes;
	
	for(unsigned int i=0; i<dataset->size(); i++)
		classes.push_back(classify(dataset->getDataInstanceAt(i)));
	
	return classes;
}

double Classifier::classificationError(const LabeledDataset * dataset, vector<double> &weights) const{
		
	assert(dataset->size() == weights.size());	
	double error_mass = 0.0;
	double total_mass = 0.0;
		
	for(unsigned int i=0; i<dataset->size(); i++)
		{
			int label_prediction = classify(dataset->getDataInstanceAt(i)); 
			
			if (label_prediction != dataset->getLabelAt(i))
				error_mass += weights[i];
				
			assert(total_mass + weights[i] >= total_mass);  // check for overflow of total mass or existence of negative weights
			total_mass += weights[i];
		}
		
	double classification_error = error_mass / total_mass;
	
	return classification_error;
}

double Classifier::classificationError(const LabeledDataset * dataset) const{

	int nsamples = dataset->size();
	vector<double> uniform_weights(nsamples, 1.0/nsamples);
	return classificationError(dataset, uniform_weights);

}

