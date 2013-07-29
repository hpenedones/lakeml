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



#include <algorithm>
#include <float.h>
#include <math.h>
#include <ThresholdLearner.h>


ThresholdLearner::ThresholdLearner()
{
}

ThresholdLearner::ThresholdLearner( FeatureExtractor * feature_extractor_) : 
feature_extractor(feature_extractor_),
optimal_threshold(0),
label_on_left(-1)
{	
}


void ThresholdLearner::train(const LabeledDataset * training_dataset, vector<double> &all_data_weights)
{
	assert(training_dataset != NULL);
	assert(training_dataset->numDiffLabels() == 2);
	assert(training_dataset->labelIsAllowed(-1));
	assert(training_dataset->labelIsAllowed(1));
	assert(training_dataset->size() > 0);
	assert(training_dataset->size() == all_data_weights.size());
		
	vector< pair<double, int> > feature_vals;
	vector<int> true_labels;
	vector<double> data_weights;
	
	double min_error = DBL_MAX;
	
	// compute feature (real-valued number) for each data sample
	int nsamples = 0;
	for(unsigned int i=0; i<training_dataset->size(); i++)
	{
		double fval = feature_extractor->getFeatureVal(training_dataset->getDataInstanceAt(i));

		cout  << "[ThresholdLearner] fval = " << fval << endl;
		if (isfinite(fval) )  // discard samples where feature is N.A. 
			{
				feature_vals.push_back( pair<double, int>(fval, nsamples));
				true_labels.push_back(training_dataset->getLabelAt(i));	
				data_weights.push_back(all_data_weights[i]);	
				nsamples++;

			}
	}	
	

	if( nsamples == 0)
		{
			//cout << "[ThresholdLearner] Warning nsamples = 0" << endl;
			// put arbitrary values and exit (it will not be selected anyways and
			// even if it was classifier would respond always zero)
			min_error = DBL_MAX;
			optimal_threshold = 0.0;
			label_on_left = 1;
			return;
		}	
	// sort according to feature value
	sort(feature_vals.begin(), feature_vals.end());  	

	// DEBUG code
	{
	// for(size_t i = 0; i < nsamples; ++i)
	// {
	// 	cout << "[ThresholdLearner] fval  = " << feature_vals[i].first << 
	// 		                      " label = " << true_labels[feature_vals[i].second] <<
	// 							  " weight = " << data_weights[feature_vals[i].second] << endl;
	// 	
	// }
	}
	
	// initialize variables
	double false_positives_if_inc = 0, false_negatives_if_inc = 0;		// if negatives are on the left of decision threshold (increasing -1 : 1)
	double false_positives_if_dec = 0, false_negatives_if_dec = 0;		// if positives are on the left of decision threshold (decreasing 1 : -1)
	
	for(int i=0; i<nsamples; i++)
		if ( true_labels[i] < 0)
			false_positives_if_inc += data_weights[i];
		else
			false_negatives_if_dec += data_weights[i];
	
	optimal_threshold = feature_vals[0].first;
	
		
	if (false_positives_if_inc < false_negatives_if_dec)
		{
			min_error = false_positives_if_inc;
			label_on_left = -1; 
		}
	else
		{
			min_error = false_negatives_if_dec;
			label_on_left = 1; 
		}
		
		
	// find split of minimum error 
	for(int i=1; i<nsamples; i++)
	{		
		int curr_ind = feature_vals[i].second;
		
		if (true_labels[curr_ind] < 0)
			{
				false_positives_if_inc -= data_weights[curr_ind];   
				false_positives_if_dec += data_weights[curr_ind];
			}
			else
			{
				false_negatives_if_inc += data_weights[curr_ind];
				false_negatives_if_dec -= data_weights[curr_ind];
			}
		
		if ( (feature_vals[i].first > optimal_threshold) )  // otherwise can not use this value to split between samples
		{
			double error_if_inc = false_positives_if_inc + false_negatives_if_inc;
			double error_if_dec = false_positives_if_dec + false_negatives_if_dec;
						
			if (error_if_inc < min_error)
			{
				label_on_left = -1;
				min_error = error_if_inc;
				optimal_threshold = feature_vals[i].first;
			}
			
			if (error_if_dec < min_error)
			{
				label_on_left = 1;
				min_error = error_if_dec;
				optimal_threshold = feature_vals[i].first;
			}
		}
	}
	
	
	// cout << "[ThresholdLearner] optimal_threshold = " << optimal_threshold << endl;
	// cout << "[ThresholdLearner] min_error = " << min_error << endl;
	// cout << "[ThresholdLearner] label_on_left = " << label_on_left << endl;
	// 
	
}


double ThresholdLearner::response(const DataInstance * data_instance) const
{
	double fval = feature_extractor->getFeatureVal(data_instance);
	
	if(!isfinite(fval))
		return fval;
	
	return (fval - optimal_threshold);
}

int	ThresholdLearner::classify(const DataInstance * data_instance) const 
{
	double resp = response(data_instance);
	
	if (!isfinite(resp)) return 0;
	
	return (resp < 0) ? label_on_left : -label_on_left;
	
}



