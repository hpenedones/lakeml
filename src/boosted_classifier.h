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




#ifndef BOOSTEDLEARNER
#define BOOSTEDLEARNER

#include <iostream>
#include <vector>

#include "classifier.h"
#include "classifier_factory.h"
#include "exponential_loss.h"

using namespace std;

/// Linear combination of classifiers (weak learners) trained by the AdaBoost algorithm
class BoostedClassifier : public Classifier {

public:

    BoostedClassifier();
    BoostedClassifier(const ClassifierFactory * classifier_factory, int max_weak_learners, int weak_learner_trials);

    int  getNumWeakLearners();

    // declared virtual in Classifier
    void   train(const Dataset & training_dataset, const vector<double> &weights);
    int    classify(const DataInstance & data_instance) const;
    double response(const DataInstance & data_instance) const;
    // response using only the part of the weak learners
    double response(const DataInstance & data_instance, int first_weak_learner, int nb_weak_learners) const;

private:

    // parameters of learning algorithm
    const ExponentialLoss * loss_function;
    const ClassifierFactory * classifier_factory;
    int learners_to_add;
    int trials_per_learner;

    // vectors used training
    vector<double> responses, curr_data_weights;
    vector<int> curr_weak_learner_predictions, best_weak_learner_predictions;

    // results of training the boosted classifier
    vector<double> weak_learners_weights;
    vector<Classifier *> weak_learners;
    double decision_threshold;
};

#endif


