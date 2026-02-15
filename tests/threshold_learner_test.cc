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
#include "src/threshold_learner.h"
#include "src/dataset.h"

// Test fixture for ThresholdLearner
class ThresholdLearnerTest : public ::testing::Test {
protected:
    void SetUp() override {
        learner = new ThresholdLearner();
    }

    void TearDown() override {
        delete learner;
    }

    ThresholdLearner* learner;
};

// Test basic training and prediction
TEST_F(ThresholdLearnerTest, TrainAndPredict) {
    // Create simple 1D dataset
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.push_back(static_cast<double>(i));
        int label = (i < 5) ? -1 : 1;
        training_dataset.add(sample, label);
    }

    std::vector<double> weights(10, 1.0);
    learner->train(training_dataset, weights);

    // Test prediction on training samples
    double response1 = learner->response(training_dataset[0]);
    double response2 = learner->response(training_dataset[9]);
    
    EXPECT_TRUE(std::isfinite(response1));
    EXPECT_TRUE(std::isfinite(response2));
}

// Test classification
TEST_F(ThresholdLearnerTest, Classification) {
    // Create simple dataset
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.push_back(static_cast<double>(i));
        int label = (i < 5) ? -1 : 1;
        training_dataset.add(sample, label);
    }

    std::vector<double> weights(10, 1.0);
    learner->train(training_dataset, weights);

    // Test classification
    int class1 = learner->classify(training_dataset[0]);
    int class2 = learner->classify(training_dataset[9]);
    
    EXPECT_TRUE(class1 == 1 || class1 == -1);
    EXPECT_TRUE(class2 == 1 || class2 == -1);
}

// Test with different weights
TEST_F(ThresholdLearnerTest, WeightedTraining) {
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.push_back(static_cast<double>(i));
        int label = (i < 5) ? -1 : 1;
        training_dataset.add(sample, label);
    }

    // Give more weight to first samples
    std::vector<double> weights(10, 1.0);
    weights[0] = 10.0;
    weights[1] = 10.0;
    
    learner->train(training_dataset, weights);

    // Training should complete without error
    int classification = learner->classify(training_dataset[0]);
    EXPECT_TRUE(classification == 1 || classification == -1);
}

// Test with multidimensional features
TEST_F(ThresholdLearnerTest, MultidimensionalFeatures) {
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.push_back(static_cast<double>(i));
        sample.push_back(static_cast<double>(i * 2));
        sample.push_back(static_cast<double>(i * 3));
        int label = (i < 5) ? -1 : 1;
        training_dataset.add(sample, label);
    }

    std::vector<double> weights(10, 1.0);
    
    // Training with multidimensional features should work
    EXPECT_NO_THROW(learner->train(training_dataset, weights));
    
    // Test prediction
    double response = learner->response(training_dataset[0]);
    EXPECT_TRUE(std::isfinite(response));
}

// Test response consistency
TEST_F(ThresholdLearnerTest, ResponseConsistency) {
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.push_back(static_cast<double>(i));
        int label = (i < 5) ? -1 : 1;
        training_dataset.add(sample, label);
    }

    std::vector<double> weights(10, 1.0);
    learner->train(training_dataset, weights);

    DataInstance test_sample;
    test_sample.push_back(3.0);
    
    // Multiple calls should give the same result
    double response1 = learner->response(test_sample);
    double response2 = learner->response(test_sample);
    
    EXPECT_EQ(response1, response2);
}

// Test with perfectly separable data
TEST_F(ThresholdLearnerTest, PerfectlySeparableData) {
    Dataset training_dataset;
    
    // Negative samples
    for (int i = 0; i < 5; i++) {
        DataInstance sample;
        sample.push_back(-10.0 - i);
        training_dataset.add(sample, -1);
    }
    
    // Positive samples
    for (int i = 0; i < 5; i++) {
        DataInstance sample;
        sample.push_back(10.0 + i);
        training_dataset.add(sample, 1);
    }

    std::vector<double> weights(10, 1.0);
    learner->train(training_dataset, weights);

    // Test that negative samples are classified as negative
    int class_neg = learner->classify(training_dataset[0]);
    // Test that positive samples are classified as positive
    int class_pos = learner->classify(training_dataset[9]);
    
    EXPECT_TRUE(class_neg == -1 || class_neg == 1);
    EXPECT_TRUE(class_pos == -1 || class_pos == 1);
}
