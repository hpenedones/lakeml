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
#include "src/gaussian_mixture_model.h"
#include "src/dataset.h"

// Test fixture for GaussianMixtureModel
class GaussianMixtureModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create GMM with 2 Gaussians and 10 max iterations
        gmm = new GaussianMixtureModel(2, 10);
    }

    void TearDown() override {
        delete gmm;
    }

    GaussianMixtureModel* gmm;
};

// Test basic training
TEST_F(GaussianMixtureModelTest, BasicTraining) {
    // Create simple 2D dataset with two clusters
    Dataset training_dataset;
    
    // First cluster around (1, 1)
    for (int i = 0; i < 5; i++) {
        DataInstance sample;
        sample.features.push_back(1.0 + i * 0.1);
        sample.features.push_back(1.0 + i * 0.1);
        sample.label = 1;
        training_dataset.push_back(sample);
    }
    
    // Second cluster around (-1, -1)
    for (int i = 0; i < 5; i++) {
        DataInstance sample;
        sample.features.push_back(-1.0 - i * 0.1);
        sample.features.push_back(-1.0 - i * 0.1);
        sample.label = -1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    
    // Training should not crash
    EXPECT_NO_THROW(gmm->train(training_dataset, weights));
}

// Test classification after training
TEST_F(GaussianMixtureModelTest, ClassifyAfterTraining) {
    // Create simple dataset
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    gmm->train(training_dataset, weights);

    // Test classification on a sample
    DataInstance test_sample;
    test_sample.features.push_back(3.0);
    test_sample.features.push_back(3.0);
    
    int classification = gmm->classify(test_sample);
    
    // Classification should be 1 or -1
    EXPECT_TRUE(classification == 1 || classification == -1);
}

// Test response after training
TEST_F(GaussianMixtureModelTest, ResponseAfterTraining) {
    // Create simple dataset
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    gmm->train(training_dataset, weights);

    // Test response on a sample
    DataInstance test_sample;
    test_sample.features.push_back(3.0);
    
    double response = gmm->response(test_sample);
    
    // Response should be a finite number
    EXPECT_TRUE(std::isfinite(response));
}

// Test with different number of Gaussians
TEST_F(GaussianMixtureModelTest, DifferentNumberOfGaussians) {
    GaussianMixtureModel gmm3(3, 5);
    
    Dataset training_dataset;
    for (int i = 0; i < 15; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(15, 1.0);
    
    // Training with 3 Gaussians should work
    EXPECT_NO_THROW(gmm3.train(training_dataset, weights));
}

// Test consistency of classification
TEST_F(GaussianMixtureModelTest, ClassificationConsistency) {
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    gmm->train(training_dataset, weights);

    DataInstance test_sample;
    test_sample.features.push_back(3.0);
    
    // Multiple calls should give the same result
    int class1 = gmm->classify(test_sample);
    int class2 = gmm->classify(test_sample);
    
    EXPECT_EQ(class1, class2);
}

// Test with multidimensional data
TEST_F(GaussianMixtureModelTest, MultidimensionalData) {
    Dataset training_dataset;
    
    for (int i = 0; i < 10; i++) {
        DataInstance sample;
        sample.features.push_back(static_cast<double>(i));
        sample.features.push_back(static_cast<double>(i * 2));
        sample.features.push_back(static_cast<double>(i * 3));
        sample.label = (i < 5) ? -1 : 1;
        training_dataset.push_back(sample);
    }

    std::vector<double> weights(10, 1.0);
    
    // Training with 3D data should work
    EXPECT_NO_THROW(gmm->train(training_dataset, weights));
}
