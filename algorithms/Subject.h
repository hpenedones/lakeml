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
#include <Observer.h>


#ifndef SUBJECT_H_FBLM31N8
#define SUBJECT_H_FBLM31N8

using namespace std;

class Subject
{
public:
//	Subject ();
//	virtual ~Subject ();

	void attach(Observer * new_observer)
	{
		observers.push_back(new_observer);
	}
	
	void notify() 
	{
	  for (unsigned int i = 0; i < observers.size(); i++)
	    observers[i]->update();
	}
	
private:
	
	vector<  Observer * > observers;
	
};

#endif 
