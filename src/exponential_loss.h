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

#include <vector>

#include "dataset.h"
#include "loss.h"

class ExponentialLoss : Loss
{
public:

    // computes the value of the loss function for a given dataset and responses of a classifier on those samples
    void value( const Dataset & dataset,
                const std::vector<double> & data_weights,
                const std::vector<double> & responses,
                std::vector<double> & out_loss) const;

    // computes the gradient of the loss function (which is a vector with one dimension per sample in the dataset)
    void gradient( const Dataset & dataset,
                   const std::vector<double> & data_weights,
                   const std::vector<double> & responses,
                   std::vector<double> & out_gradient) const;

    // Computes how far one should move along a given direction in order to minimize the loss the most.
    // It puts its return values in the doubles of the last 2 arguments, for which we have the pointers.
    void optimal_step_along_direction(const Dataset & dataset,
                                      const std::vector<double> & data_weights,
                                      const std::vector<double> & responses,
                                      const std::vector<int> & direction,
                                      double * out_optimal_step,
                                      double * out_minimum_loss) const;

private:

};