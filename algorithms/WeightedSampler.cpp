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



#include <iostream>
#include <WeightedSampler.h>
#include <vector>
#include <fstream>

using namespace std;


double WeightedSampler::sample(vector<double> &weights, int nb_to_sample, vector<int> &sampled, vector<double> &frequencies)	
{
	// vectors should already come with memory allocated
	assert((int) sampled.size() == nb_to_sample);
	assert((int) frequencies.size() == nb_to_sample);
		
	vector<int> round_sampled(nb_to_sample);
	vector<int> sample_n_occurences(weights.size(),0);
		
		
	for(int i=0; i<nb_to_sample; i++)
		frequencies[i] = 0.0;
		
	int ndiff = 0;
	double mass = 0.0;
	
	int round = 0;
	while(ndiff < nb_to_sample)
	{
	
	mass = robust_sampling(weights, nb_to_sample, round_sampled);
	
	for(unsigned int i=0; i< round_sampled.size() && ndiff < nb_to_sample; i++)
		{
			sample_n_occurences[round_sampled[i]]++;
			if (sample_n_occurences[round_sampled[i]] == 1)
				ndiff++;
		}
		round++;
	}
	cout << "[WeightedSampler]" << "robust sampling rounds = " <<  round << endl;	
	
	assert(mass > 0);
	
	double sampled_mass = 0.0;
	for(int i=0, j=0; i< (int) sample_n_occurences.size() && j<nb_to_sample; i++)
	{
		if (sample_n_occurences[i] > 0)
			{
			 sampled[j] = i;
			frequencies[j] = sample_n_occurences[i];
			sampled_mass +=  frequencies[j];
			j++;
			
			}
	}
	
	for(unsigned int i=0; i<frequencies.size();i++)
	{
		frequencies[i] /= sampled_mass;
	//	cout << " sample " << sampled[i] << " occurences " << frequencies[i] << endl;
		
	}
	
	return mass;
}

// function that samples <nb_to_sample> examples according to the distribution given by <weights>
// Note that the return value = sum(weights)
// adapted from F.Fleuret's code from http://www.idiap.ch/folded-ctf/
double WeightedSampler::robust_sampling(vector<double> &weights, int nb_to_sample, vector<int> &sampled) 
{
 int nb = weights.size();
 
 assert( nb > 0);
	
  if(nb == 1) 
	{
    for(int k = 0; k < nb_to_sample; k++) 
		sampled[k] = 0;
    return weights[0];
  	}
  else 
	{
	vector<double> pair_weights((nb+1)/2);
	
	for(int k = 0; k < nb/2; k++)
	      pair_weights[k] = weights[2 * k] + weights[2 * k + 1];
	
	if (nb%2)
	   pair_weights[(nb+1)/2 - 1] = weights[nb-1];
	
	double result = robust_sampling(pair_weights, nb_to_sample, sampled);
	
	for(int k = 0; k < nb_to_sample; k++) {
	      int s = sampled[k];
	      if(drand48() * pair_weights[s] <= weights[2 * s])
	        sampled[k] = 2 * s;
	      else
	        sampled[k] = 2 * s + 1;
	    }
    return result;
    }
}
	
	


