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

#include <gtest/gtest.h>
#include <vector>
#include "src/boosted_classifier.h"
#include "src/threshold_learner.h"
#include "src/classifier_factory.h"
#include "src/dataset.h"

// Simple factory for creating threshold learners
class ThresholdLearnerFactory : public ClassifierFactory {
public:
    Classifier* create() const override {
        return new ThresholdLearner();
    }
};

// Test fixture for BoostedClassifier
class BoostedClassifierTest : public ::testing::Test {
protected:
    void SetUp() override {
        factory = new ThresholdLearnerFactory();
        // Create boosted classifier with 5 weak learners and 10 trials each
        classifier = new BoostedClassifier(factory, 5, 10);
    }

    void TearDown() override {
        delete classifier;
        delete factory;
    }

    ClassifierFactory* factory;
    BoostedClassifier* classifier;
};

// Test basic training and prediction
TEST_F(BoostedClassifierTest, TrainAndPredict) {
    // Create simple linearly separable dataset
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    classifier->train(training_dataset, weights);

    // Check that weak learners were added
    EXPECT_GT(classifier->getNumWeakLearners(), 0);
    EXPECT_LE(classifier->getNumWeakLearners(), 5);

    // Test prediction on training sample
    double response = classifier->response(training_dataset[0]);
    int classification = classifier->classify(training_dataset[0]);
    
    EXPECT_TRUE(std::isfinite(response));
    EXPECT_TRUE(classification == 1 || classification == -1);
}

// Test getNumWeakLearners
TEST_F(BoostedClassifierTest, GetNumWeakLearners) {
    // Before training, should have 0 weak learners
    EXPECT_EQ(classifier->getNumWeakLearners(), 0);

    // Create simple dataset
    Dataset training_dataset;
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    classifier->train(training_dataset, weights);

    // After training, should have some weak learners
    EXPECT_GT(classifier->getNumWeakLearners(), 0);
}

// Test response with specific weak learners
TEST_F(BoostedClassifierTest, ResponseWithWeakLearnerRange) {
    // Create simple dataset
    Dataset training_dataset;
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    classifier->train(training_dataset, weights);

    int num_learners = classifier->getNumWeakLearners();
    if (num_learners > 0) {
        DataInstance test_sample;
        test_sample.features.push_back(3.0);
        test_sample.label = -1;

        // Test response using only first weak learner
        double response = classifier->response(test_sample, 0, 1);
        EXPECT_TRUE(std::isfinite(response));
    }
}

// Test classification consistency
TEST_F(BoostedClassifierTest, ClassificationConsistency) {
    // Create simple dataset
    Dataset training_dataset;
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    classifier->train(training_dataset, weights);

    DataInstance test_sample;
    test_sample.features.push_back(3.0);
    test_sample.label = -1;

    // Multiple calls should give the same result
    int class1 = classifier->classify(test_sample);
    int class2 = classifier->classify(test_sample);
    
    EXPECT_EQ(class1, class2);
}

// Test with uniform weights
TEST_F(BoostedClassifierTest, UniformWeights) {
    Dataset training_dataset;
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    classifier->train(training_dataset, weights);

    EXPECT_GT(classifier->getNumWeakLearners(), 0);
}
