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
#include "src/classifier.h"
#include "src/threshold_learner.h"
#include "src/dataset.h"

// Test fixture for Classifier base functionality
class ClassifierTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple threshold learner for testing
        classifier = new ThresholdLearner();
    }

    void TearDown() override {
        delete classifier;
    }

    Classifier* classifier;
};

// Test batch response method
TEST_F(ClassifierTest, BatchResponse) {
    // Create simple training dataset
    Dataset training_dataset;
    DataInstance sample1, sample2;
    sample1.features.push_back(1.0);
    sample1.label = 1;
    sample2.features.push_back(-1.0);
    sample2.label = -1;
    training_dataset.push_back(sample1);
    training_dataset.push_back(sample2);

    std::vector<double> weights = {1.0, 1.0};
    classifier->train(training_dataset, weights);

    // Test batch response on training data
    std::vector<double> responses = classifier->response(training_dataset);
    
    ASSERT_EQ(responses.size(), 2);
    // Response should be computed for all samples
    EXPECT_NE(responses[0], 0.0);
    EXPECT_NE(responses[1], 0.0);
}

// Test batch classify method
TEST_F(ClassifierTest, BatchClassify) {
    // Create simple training dataset
    Dataset training_dataset;
    DataInstance sample1, sample2;
    sample1.features.push_back(1.0);
    sample1.label = 1;
    sample2.features.push_back(-1.0);
    sample2.label = -1;
    training_dataset.push_back(sample1);
    training_dataset.push_back(sample2);

    std::vector<double> weights = {1.0, 1.0};
    classifier->train(training_dataset, weights);

    // Test batch classification on training data
    std::vector<int> classes = classifier->classify(training_dataset);
    
    ASSERT_EQ(classes.size(), 2);
    // Classes should be either 1 or -1
    EXPECT_TRUE(classes[0] == 1 || classes[0] == -1);
    EXPECT_TRUE(classes[1] == 1 || classes[1] == -1);
}

// Test with empty dataset
TEST_F(ClassifierTest, EmptyDataset) {
    Dataset empty_dataset;
    
    std::vector<double> responses = classifier->response(empty_dataset);
    std::vector<int> classes = classifier->classify(empty_dataset);
    
    EXPECT_EQ(responses.size(), 0);
    EXPECT_EQ(classes.size(), 0);
}

// Test single sample response and classification
TEST_F(ClassifierTest, SingleSample) {
    // Create training dataset
    Dataset training_dataset;
    DataInstance sample1, sample2;
    sample1.features.push_back(1.0);
    sample1.label = 1;
    sample2.features.push_back(-1.0);
    sample2.label = -1;
    training_dataset.push_back(sample1);
    training_dataset.push_back(sample2);

    std::vector<double> weights = {1.0, 1.0};
    classifier->train(training_dataset, weights);

    // Test single sample
    DataInstance test_sample;
    test_sample.features.push_back(0.5);
    test_sample.label = 1;
    
    double response = classifier->response(test_sample);
    int classification = classifier->classify(test_sample);
    
    // Response should be a real number
    EXPECT_TRUE(std::isfinite(response));
    // Classification should be 1 or -1
    EXPECT_TRUE(classification == 1 || classification == -1);
}
