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
#include <math.h>

#include "naive_bayes_classifier.h"
#include "math_utils.h"

using namespace std;

NaiveBayesClassifier::NaiveBayesClassifier()
{
}

NaiveBayesClassifier::~NaiveBayesClassifier()
{
    for (size_t i = 0; i < weak_learners.size(); i++)
    {
        delete weak_learners[i];
    }
}


NaiveBayesClassifier::NaiveBayesClassifier(const ClassifierFactory* classifier_factory_, int num_weak_learners_):
    classifier_factory(classifier_factory_), learners_to_add(num_weak_learners_), decision_threshold(0.0)
{

}


void NaiveBayesClassifier::train(const Dataset & training_dataset, const vector<double> &weights)
{

    for (int i = 0; i < learners_to_add; i++)
    {
        Classifier * next_weak_learner = classifier_factory->createRandomInstance();
        next_weak_learner->train(training_dataset, weights);
        weak_learners.push_back(next_weak_learner);
    }

    vector<double> pos_responses, neg_responses;

    for (size_t i = 0; i < training_dataset.size(); i++)
    {
        double my_resp = response(training_dataset[i]);

        if (training_dataset.getLabelAt(i) == 1)
            pos_responses.push_back(my_resp);
        else
            neg_responses.push_back(my_resp);
    }

    /*
        \fixme this is an arbitrary choice
    */
    decision_threshold = *min_element(pos_responses.begin(), pos_responses.end());

    int true_negatives = 0, false_positives = 0;

    for (size_t i = 0; i < neg_responses.size(); i++)
    {
        if ( neg_responses[i] > decision_threshold)
            false_positives++;
        else
            true_negatives++;
    }
}

double NaiveBayesClassifier::response(const DataInstance & data_instance) const
{
    double resp = 0.0;
    int valid_responses = 0;
    /*
        \todo Here we are assuming that the responses of the weak learners are: log(p(x| y=1)) - log(p(x|y=0))
    */
    for (size_t i = 0; i < weak_learners.size(); i++)
    {
        double fval = weak_learners[i]->response(data_instance);

        if (isfinite(fval))
        {
            resp += fval;
            valid_responses++;
        }
    }

    return resp / valid_responses;
}

int NaiveBayesClassifier::classify(const DataInstance & data_instance) const
{
    double resp = response(data_instance);

    if (resp > decision_threshold)
        return 1;

    return -1;

}

