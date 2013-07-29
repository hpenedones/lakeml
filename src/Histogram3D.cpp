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



#include <Histogram3D.h>
#include <assert.h>
#include <math.h>

Histogram3D::Histogram3D (int nbins, int upper_limit)
{
	this->total_mass = 0;
	this->nbins = nbins;
	this->upper_limit = upper_limit;


	hist = new double[nbins * nbins * nbins];

	for(int j=0; j< nbins * nbins * nbins;j++)
	{
		hist[j] = residual_mass;			 // initalize with a uniform distribution (prior)
		total_mass += hist[j];	 			 // given its small mass, it will fast vanish after the first data arrives
	}

	bin_size = (upper_limit + 1.0) / nbins;
}

Histogram3D::~Histogram3D()
{
	delete [] hist;
}

	// kernel density estimation (using a cube of the same dimension as the histogram bins)
void Histogram3D::addPoint(int x, int y, int z, double weight)
{

	if ( !(x>=0 && x<=upper_limit) || !(y>=0 && y<=upper_limit) || !(z>=0 && z<=upper_limit))
		cout  << " x = " << x << " y = " << y << " z = " << z << " upper_limit = " << upper_limit << endl;
	
	assert(x>=0 && x<=upper_limit);
	assert(y>=0 && y<=upper_limit);
	assert(z>=0 && z<=upper_limit);

	float inters_mass;

	int indX[2], indY[2],indZ[2];
	float fracB[2],fracG[2],fracR[2];


		// find histogram bins coliding with data point
	indX[0] = floor(x/bin_size);
	indX[1] = (x > (indX[0]+0.5)*bin_size) ? indX[0]+1 : indX[0]-1 ;

	indY[0] = floor(y/bin_size);
	indY[1] = (y > (indY[0]+0.5)*bin_size) ? indY[0]+1 : indY[0]-1 ;

	indZ[0] = floor(z/bin_size);
	indZ[1] = (z > (indZ[0]+0.5)*bin_size) ? indZ[0]+1 : indZ[0]-1 ;


	fracB[0] = x/bin_size-(min(indX[0], indX[1])+0.5);
	fracB[1] = 1 - fracB[0];

	fracG[0] = y/bin_size-(min(indY[0], indY[1])+0.5);
	fracG[1] = 1 - fracG[0];

	fracR[0] = z/bin_size-(min(indZ[0], indZ[1])+0.5);
	fracR[1] = 1 - fracR[0];

		// compute intersection volume with 3D histogram
	double mass_inside_hist = 0.0;
	for(int b=0; b<=1; b++)
		if (indX[b] >= 0 && indX[b] < nbins)
		for(int g=0;g<=1; g++)
		if (indY[g] >= 0 && indY[g] < nbins)
		for(int r=0;r<=1; r++)
		if(indZ[r] >= 0 && indZ[r] < nbins)
	{
		mass_inside_hist += weight*fracB[b]*fracG[g]*fracR[r];
	}
		// add mass to histogram bins according to intersection volumes					
	for(int b=0; b<=1; b++)
		if (indX[b] >= 0 && indX[b] < nbins)
		for(int g=0;g<=1; g++)
		if (indY[g] >= 0 && indY[g] < nbins)
		for(int r=0;r<=1; r++)
		if(indZ[r] >= 0 && indZ[r] < nbins)
	{
		inters_mass = weight*fracB[b]*fracG[g]*fracR[r];
		hist[indX[b]*nbins*nbins + indY[g]*nbins + indZ[r]] += inters_mass/mass_inside_hist;
		total_mass += inters_mass/mass_inside_hist;

	}


}

double Histogram3D::getPointProbability(int x, int y, int z)
{
	assert(x>=0 && x<=upper_limit);
	assert(y>=0 && y<=upper_limit);
	assert(z>=0 && z<=upper_limit);

	int ind_B = floor(x/(upper_limit + numerical_delta)* nbins);
	int ind_G = floor(y/(upper_limit + numerical_delta)* nbins);
	int ind_R = floor(z/(upper_limit + numerical_delta)* nbins);

	double count = hist[ind_B*nbins*nbins + ind_G*nbins +ind_R];

	double point_probability = count/total_mass;

	return point_probability;

}




