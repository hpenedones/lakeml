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
#include "src/exponential_loss.h"
#include "src/dataset.h"

// Test fixture for ExponentialLoss
class ExponentialLossTest : public ::testing::Test {
protected:
    void SetUp() override {
        loss = new ExponentialLoss();
    }

    void TearDown() override {
        delete loss;
    }

    ExponentialLoss* loss;
};

// Test basic loss value computation
TEST_F(ExponentialLossTest, ComputeLossValue) {
    // Create simple dataset with 2 samples
    Dataset dataset;
    DataInstance sample1, sample2;
    sample1.push_back(1.0);
    sample2.push_back(-1.0);
    dataset.add(sample1, 1);
    dataset.add(sample2, -1);

    std::vector<double> weights = {1.0, 1.0};
    std::vector<double> responses = {0.5, -0.5};
    std::vector<double> out_loss;

    loss->value(dataset, weights, responses, out_loss);

    ASSERT_EQ(out_loss.size(), 2);
    // Exponential loss should be positive
    EXPECT_GT(out_loss[0], 0.0);
    EXPECT_GT(out_loss[1], 0.0);
}

// Test gradient computation
TEST_F(ExponentialLossTest, ComputeGradient) {
    // Create simple dataset with 2 samples
    Dataset dataset;
    DataInstance sample1, sample2;
    sample1.push_back(1.0);
    sample2.push_back(-1.0);
    dataset.add(sample1, 1);
    dataset.add(sample2, -1);

    std::vector<double> weights = {1.0, 1.0};
    std::vector<double> responses = {0.5, -0.5};
    std::vector<double> out_gradient;

    loss->gradient(dataset, weights, responses, out_gradient);

    ASSERT_EQ(out_gradient.size(), 2);
    // Gradient should be computed for both samples
    EXPECT_NE(out_gradient[0], 0.0);
    EXPECT_NE(out_gradient[1], 0.0);
}

// Test with empty dataset
TEST_F(ExponentialLossTest, EmptyDataset) {
    Dataset dataset;
    std::vector<double> weights;
    std::vector<double> responses;
    std::vector<double> out_loss;

    loss->value(dataset, weights, responses, out_loss);

    EXPECT_EQ(out_loss.size(), 0);
}

// Test with zero weights
TEST_F(ExponentialLossTest, ZeroWeights) {
    Dataset dataset;
    DataInstance sample;
    sample.push_back(1.0);
    dataset.add(sample, 1);

    std::vector<double> weights = {0.0};
    std::vector<double> responses = {0.5};
    std::vector<double> out_loss;

    loss->value(dataset, weights, responses, out_loss);

    ASSERT_EQ(out_loss.size(), 1);
    // Loss with zero weight should be zero
    EXPECT_EQ(out_loss[0], 0.0);
}
