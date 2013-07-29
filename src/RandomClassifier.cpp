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



#include <RandomClassifier.h>

RandomClassifier::RandomClassifier(double fraction_accept_): fraction_accept(fraction_accept_)
{

}

RandomClassifier::~RandomClassifier() 
{

}

void RandomClassifier::train(const LabeledDataset* training_dataset, vector<double> &weights) 
{

}

double RandomClassifier::response(const DataInstance * data_instance) const 
{

	if (drand48() < fraction_accept)
		return 10000.0;
	
	return -10000.0;
}

int RandomClassifier::classify(const DataInstance * data_instance) const 
{
	if (drand48() < fraction_accept)
		return 1;
	
	return -1;
}

