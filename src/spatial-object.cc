/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gtk-visualization
 * Copyright (C) 2013 Michael Blackmon <burned@pluto>
 * 
 * gtk-visualization is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gtk-visualization is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "spatial-object.h"
#include "common.h"

#include <iostream>

using std::cout;
using std::endl;

void dump(SpatialObject *p) {
	cout << p->type() << '\t' << p->X[0] << '\t' << p->X[1] << '\t' << p->X[2] << '\t'
		 << p->dX[0] << '\t' << p->dX[1] << '\t' << p->dX[2] << '\t'
		 << p->F[0] << '\t' << p->F[1] << '\t' << p->F[2] << '\t'
		 << p->mass << endl;
}


void SpatialObject::rotate_frame(double theta_x, double theta_y, double theta_z) 
{
	double r0;

	if( theta_x != 0.0 ) 
	{ 
		M[1][1] = cos( theta_x ) * M[1][1] - sin(theta_x) * M[2][1];
		M[1][2] = cos( theta_x ) * M[1][2] - sin(theta_x) * M[2][2];

		M[2][1] = sin( theta_x ) * M[1][1] + cos(theta_x) * M[2][1];
		M[2][2] = sin( theta_x ) * M[1][2] + cos(theta_x) * M[2][2];

		r0 = NORM( M[1] );
		M[1][0] /= r0; M[1][1] /= r0; M[1][2] /= r0;
		
		r0 = NORM( M[1] );
		M[2][0] /= r0; M[2][1] /= r0; M[2][2] /= r0;
	}

	if(theta_z != 0.0) 
	{
		M[0][0] = cos( theta_z ) * M[0][0] + sin(theta_z) * M[1][0];
		M[0][1] = cos( theta_z ) * M[0][1] + sin(theta_z) * M[1][1];

		M[1][0] = -sin( theta_z ) * M[0][0] + cos(theta_z) * M[1][0];
		M[1][1] = -sin( theta_z ) * M[0][1] + cos(theta_z) * M[1][1];

		r0 = NORM( M[0] );
		M[0][0] /= r0; M[0][1] /= r0; M[0][2] /= r0;
		
		r0 = NORM( M[1] );
		M[1][0] /= r0; M[1][1] /= r0; M[1][2] /= r0;
	}

	if(theta_y != 0.0)
	{
		M[0][0] = cos( theta_y ) * M[0][0] - sin(theta_y) * M[2][0];
		M[0][2] = cos( theta_y ) * M[0][2] - sin(theta_y) * M[2][2];

		M[2][0] = sin( theta_y ) * M[0][0] + cos(theta_y) * M[2][0];
		M[2][2] = sin( theta_y ) * M[0][2] + cos(theta_y) * M[2][2];

		r0 = NORM( M[0] );
		M[0][0] /= r0; M[0][1] /= r0; M[0][2] /= r0;
		
		r0 = NORM( M[2] );
		M[2][0] /= r0; M[2][1] /= r0; M[2][2] /= r0;
	}
}

