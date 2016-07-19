/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gtk-visualization
 * Copyright (C) 2013 Michael Blackmon <burned@euler>
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

#include "particle.h"
#include "common.h"
#include "math-common.h"

#include <sstream>
#include <iomanip>

std::string Particle::to_string()
{
	std::stringstream ss;

	ss << std::fixed << std::setprecision(6);
	ss  << "PARTICLE\t" << _sp_id << '\t' << _R
		<< '\t' <<  X[0] << '\t' <<  X[1] << '\t' <<  X[2] 
		<< '\t' << dX[0] << '\t' << dX[1] << '\t' << dX[2]
		<< '\t' <<  F[0] << '\t' <<  F[1] << '\t' <<  F[2];

	return ss.str();
}

bool Particle::draw( const Cairo::RefPtr< Cairo::Context > &cr )
{

	double r1, r2, r3;
	
	cr->save();
	
	if( id() >= 0 )
	{

		cr->set_line_width( 0.01 );
		
		r1 = norm(  F );
		r2 = norm( dX );

		if(r1 > 0.0)
		{
		
			cr->move_to(0.0,0.0);
			cr->set_source_rgba( 0.0, 1.0, 0.0, 0.75);

			cr->line_to( F[0] * _R / r1, F[1] * _R / r1);
			cr->stroke();
		}

		if(r2 > 0.0)
		{

			cr->move_to(0.0,0.0);
			cr->set_source_rgba( 1.0, 0.0, 0.0, 0.75);

			cr->line_to( dX[0] * _R / 10.0,  dX[1] * _R / 10.0);
			cr->stroke();
		}


		{
			cr->move_to(0.0,0.0);
			cr->set_source_rgba( 1.0, 0.0, 0.0, 0.55);

			cr->line_to( M[0][0] * _R,  M[0][1] * _R);
			cr->stroke();

			cr->move_to(0.0,0.0);
			cr->set_source_rgba( 0.0, 1.0, 0.0, 0.55);

			cr->line_to( M[1][0] * _R,  M[1][1] * _R);
			cr->stroke();

		}

		cr->move_to(_R, 0.0);
		cr->set_source_rgba( 0.0, 0.0, 1.0, 0.75);
		
		cr->arc(0, 0, _R, 0, 2*M_PI);
		cr->stroke();
		
	}
	else
	{
		cr->move_to(_R, 0.0);
		cr->arc(0, 0, _R, 0, 2*M_PI);
		cr->fill();
	}
	
	cr->restore();
	return true;

}

// return the signed distance between the particle and p
double Particle::distance_to( Vector pos ) {
	
	return (norm(X - pos) - _R);
	
}

// Update the forces acting on the particle to account for a collision
// with sp
void Particle::collid_with(SpatialObject *p) {
	double r0, r1, r2, r3, a;
	Vector u, v, w;
	
	u = p->X - X;

	r0 = norm( u );
	
	if( r0 > 0.0 )
	{
		u /= r0;
	} else
		return;

	r1 = inner_prod (u, p->dX - dX);

	r2 = p->distance_to( X );
	
	if( r1 < 0.0 || r2 < _R ) {


		r1 = (r0 - r2) + _R;

		a = 900 / (r1 * r1) * 11.589;

		r3 = POLY_O2( 1.00 - (r1*r1)/(r0*r0), 0.00, a, 0.00);
		DEBUG(r3);
		p->F += fabs(r3) * u * r0;
		
	}
	//DEBUG(r1 - 2.0 * _R);
}

void Particle::update_forces() {


}

