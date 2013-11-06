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

#include <LossFunction.h>
#include <Dataset.h>
#include <math.h>
#include <cassert>
#include <iostream>

using namespace std;

// computes the value of the loss function for a given dataset and responses of a classifier on those samples
void LossFunction::value( const Dataset & dataset,
 			const vector<double> & data_weights,
			const vector<double> & responses,
			vector<double> & out_loss) const
{
	assert(out_loss.size() == responses.size());
	assert(data_weights.size() == dataset.size());
	assert(data_weights.size() == responses.size());

	for(size_t i = 0; i < dataset.size(); i++)
	{
		out_loss[i] = data_weights[i] * exp(-dataset.getLabelAt(i)*responses[i]);
	}

}

// computes the gradient of the loss function (which is a vector with one dimension per sample in the dataset)  
void LossFunction::gradient( const Dataset & dataset,
			   const vector<double> & data_weights,
 			   const vector<double> & responses,
 			   vector<double> & out_gradient) const
{
	assert(out_gradient.size() == responses.size());
	assert(data_weights.size() == dataset.size());
	assert(data_weights.size() == responses.size());
	
	for(size_t i = 0; i < dataset.size(); i++)
	{
		out_gradient[i] = data_weights[i] * -dataset.getLabelAt(i)* exp(-dataset.getLabelAt(i)*responses[i]);
	}
	
}

// computes how far one should move along a given direction in order to minimize the loss the most 
// it puts its return values in the doubles of the last 2 arguments, for which we have the pointers
void LossFunction::optimal_step_along_direction(const Dataset & dataset,
	   							  const vector<double> & data_weights,
 								  const vector<double> & responses,
 								  const vector<int> & direction,
								  double * out_optimal_step,
								  double * out_minimum_loss) const
{
	assert(direction.size() == responses.size());
	assert(data_weights.size() == dataset.size());
	assert(data_weights.size() == responses.size());
	
	
	double W_0 = 0.0, W_minus = 0.0, W_plus = 0.0;
	
	for(size_t i = 0; i < dataset.size(); i++)
	{
		double val = data_weights[i] * exp(-dataset.getLabelAt(i)*responses[i]);
		
		switch ( direction[i]*dataset.getLabelAt(i))
		{
			case 0:
			W_0 += val;
			break;
			case -1:
			W_minus += val;
			break;
			case 1:
			W_plus += val;
			break;
			default:
			abort();

		}			
	}	

	*out_minimum_loss = W_0 + 2*sqrt(W_minus*W_plus);
	*out_optimal_step = 0.5*log(W_plus/W_minus);  		// the same as log(sqrt(W_plus/W_minus))
	
	
}
