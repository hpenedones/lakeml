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



#include <TrimmingSampler.h>
#include <algorithm>
#include <iostream>


double TrimmingSampler::sample(vector<double> &weights, int nb_to_sample, vector<int> &sampled, vector<double> &frequencies)
{
		
	// vectors should already come with memory allocated
	assert((int) sampled.size() == nb_to_sample);
	assert((int) frequencies.size() == nb_to_sample);
		
	vector< pair<double,int> > weights_indexes(weights.size());
	
	double mass = 0.0;
	for(int i=0; i< (int) weights.size(); i++)
	{
		mass += weights[i];
		weights_indexes[i] = pair<double, int>(weights[i], i);
	}
		
		
	sort(weights_indexes.begin(), weights_indexes.end());
	
	double captured_mass = 0.0;
	for(int j=0; j<nb_to_sample; j++)
		{
			int i = weights_indexes.size() - j - 1;
			sampled[j] = weights_indexes[i].second;
			frequencies[j] = weights_indexes[i].first;
			captured_mass += frequencies[j];
			
		}

	for(int i=0; i<nb_to_sample; i++)
		{
			frequencies[i] /= captured_mass;
			cout << sampled[i] << " " << frequencies[i] << endl;
			
		}
			
	return mass;	
}
