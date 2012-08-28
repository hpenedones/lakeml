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




#include <cmath>
#include <math.h>
#include <float.h>
#include <BoostedClassifier.h>

// BOOST_CLASS_EXPORT(BoostedClassifier)  // This has to come after #include <classname.h> and after #include <...archive.hpp>

#include <iostream>
#include <fstream>

using namespace std;

// extern ofstream gbl_logstream; 


BoostedClassifier::BoostedClassifier()
{
	// needed because of the serialization
}

BoostedClassifier::BoostedClassifier(const ClassifierFactory* classifier_factory_, int max_weak_learners_, int weak_learner_trials_): 
classifier_factory(classifier_factory_),
	learners_to_add(max_weak_learners_),
	trials_per_learner(weak_learner_trials_),
	weak_learners_weights(),
	weak_learners(),
	decision_threshold(0)
{

}


int BoostedClassifier::getNumWeakLearners(){

	return weak_learners.size();
}


void BoostedClassifier::train_in_batch_mode(BufferedLabeledDataset * buffered_dataset, vector<double> & initial_data_weights, int nbatches)
{
	
	cout << "Entered train_in_batch_mode" << endl;
	
	for(size_t i = 0; i < initial_data_weights.size(); i++)
	{
		big_dataset_weights.push_back(initial_data_weights[i]);
		big_dataset_responses.push_back(0.0);
	}
	
	vector<double> active_set_data_weights(buffered_dataset->getActiveSetSize());


	cout << "Initialized big_dataset_weights and big_dataset_responses" << endl;
	
	for(int batch = 0; batch < nbatches; batch++)
	{
		cout  << " batch = " << batch << endl;
		if (batch > 0)
			{
				for(size_t i = 0; i < big_dataset_responses.size(); i++)
					big_dataset_responses[i] += response(buffered_dataset->getDataInstanceAt(i), (batch-1) * learners_to_add, learners_to_add);
				
			}
			 
		// update data weights for next round
		loss_function->value(buffered_dataset, initial_data_weights, big_dataset_responses, big_dataset_weights);			
		buffered_dataset->resampleActiveDataset(big_dataset_weights, active_set_data_weights);
		
		train(buffered_dataset->getActiveDataset(), active_set_data_weights);		
	}
	
}


void BoostedClassifier::train(const LabeledDataset * training_dataset, vector<double> & initial_data_weights)
{
	// assertions
	{
		assert(classifier_factory != NULL); 
		assert(training_dataset != NULL);	
		assert(training_dataset->size() > 0);
		assert(training_dataset->numDiffLabels() == 2);
		assert(training_dataset->labelIsAllowed(1));
		assert(training_dataset->labelIsAllowed(-1));
		assert(learners_to_add > 0);
		assert(trials_per_learner > 0);
		assert(initial_data_weights.size() == training_dataset->size());
	}
	
	cout  << " training_dataset->size() = " << training_dataset->size() << endl;

	// initialize vectors with the size of the dataset
	responses.clear();
	curr_weak_learner_predictions.clear();
	best_weak_learner_predictions.clear();
	curr_data_weights.clear();
	
	for(size_t i = 0; i < training_dataset->size(); i++)
		{
			responses.push_back(0.0);
			curr_weak_learner_predictions.push_back(0);
			best_weak_learner_predictions.push_back(0);
			curr_data_weights.push_back(initial_data_weights[i]);
		}
		
	for (int wl=0; wl<learners_to_add; wl++) 
	{
		cout  << " wl = " << wl << endl;
		double min_loss = DBL_MAX, best_weak_learner_weight;
		Classifier * best_weak_learner;
		
		for(int trial=0; trial<trials_per_learner; trial++)
		{
			cout  << " trial = " << trial << endl;
			
			Classifier * current_weak_learner = classifier_factory->createRandomInstance();

			current_weak_learner->train(training_dataset, curr_data_weights);
			
			curr_weak_learner_predictions = current_weak_learner->classifications(training_dataset);

			double optimal_step, loss_after_step;
			
			loss_function->optimal_step_along_direction(training_dataset, initial_data_weights, 
														responses, curr_weak_learner_predictions, &optimal_step, &loss_after_step);

			if ( loss_after_step < min_loss )
			{
				min_loss = loss_after_step;
				best_weak_learner_weight = optimal_step;
				delete best_weak_learner;    // delete previous best
				best_weak_learner = current_weak_learner;
				best_weak_learner_predictions = curr_weak_learner_predictions;
			}
			else
				delete current_weak_learner;
		}

		assert(best_weak_learner != NULL);
		assert(isfinite(best_weak_learner_weight));
		
		// add best weak learner to strong classifier
		weak_learners.push_back(best_weak_learner);
		weak_learners_weights.push_back(best_weak_learner_weight);

		// update strong classifier responses
		for(unsigned int i=0; i<training_dataset->size(); i++)
			responses[i] += best_weak_learner_weight * best_weak_learner_predictions[i];	

		// update data weights for next round
		loss_function->value(training_dataset, initial_data_weights, responses, curr_data_weights);
		
		// notify observers that the state has changed
		notify(); 

	}
}




double BoostedClassifier::response(const DataInstance * data_instance, int first_weak_learner, int nb_weak_learners) const 
{
	assert( first_weak_learner >= 0);
	assert( first_weak_learner + nb_weak_learners <= (int) weak_learners.size());
	
	double resp = 0.0;

	for(int m=0; m < nb_weak_learners; m++)
	{
		resp += weak_learners_weights[first_weak_learner + m] * weak_learners[first_weak_learner + m]->classify(data_instance);
	}

	return resp;
}


double BoostedClassifier::response(const DataInstance * data_instance) const 
{
	double resp = 0.0;

	for(unsigned int m=0; m < weak_learners.size(); m++)
	{
		resp += weak_learners_weights[m] * weak_learners[m]->classify(data_instance);
	}

	return resp;
}


int BoostedClassifier::classify(const DataInstance * data_instance) const
{

	double resp = response(data_instance);

	return ((resp <= decision_threshold) ? -1 : 1);
}




