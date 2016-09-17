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
#include <iostream>


using namespace std;

class Histogram3D
{
public:


    Histogram3D (int nbins, int upper_limit);

    ~Histogram3D();

    // kernel density estimation (using a cube of the same dimension as the histogram bins)
    void addPoint(int x, int y, int z, double weight);

    double getPointProbability(int x, int y, int z);


private:

    static const double numerical_delta; // = 0.000001;
    static const double residual_mass;//  = 0.1;


    int upper_limit;
    int nbins;
    double * hist;
    double total_mass, bin_size;

};
