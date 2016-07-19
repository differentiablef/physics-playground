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

#include "edge.h"
#include "common.h"

#include <string>
#include <sstream>
#include <iomanip>

void Edge::_update_vars() {
	if( _A == NULL || _B == NULL )
		return;


	// Set SpatialObject Coornidates to the mid-point
	X = 0.5 * (_A->X + _B->X );

	_u = _B->X - _A->X;

	_dist_real = norm( _u );

	_scale = 0.5 * _dist_real;

	if( _dist_real > 0.0 ) {
		_n[0] =  1.0 * _u[1] / _dist_real;
		_n[1] = -1.0 * _u[0] / _dist_real;
		_n[2] = 0.0;
	}

}

std::string Edge::to_string()
{
	std::stringstream ss;

	ss << std::fixed << std::setprecision(6);
	ss << type() << '\t' << _dist << '\t' << _A->id() << '\t' << _B->id() << '\t';

	return ss.str();
}

bool Edge::draw( const Cairo::RefPtr< Cairo::Context > &cr ) {

	if( _A == NULL || _B == NULL )
		return true;

	_update_vars();
	
	cr->save();
	cr->set_line_width( 0.01 );
	cr->set_source_rgba(0.0, 0.0, 0.0, 0.75);

	cr->move_to( _A->X[0] - X[0], _A->X[1] - X[1] );
	cr->line_to( _B->X[0] - X[0], _B->X[1] - X[1] );
	cr->stroke();
	
	cr->move_to(0.0, 0.0);
	cr->line_to(_n[0] * 0.05, _n[1] * 0.05 );
	cr->stroke();
	
	cr->restore();

	return true;
}

void Edge::update_forces() 
{

	double r0;
	_update_vars();

	if( _A == NULL || _B == NULL )
		return;
	
	// Make  the local frame for B the edge and edge normal vector 

	
	_B->M[0][0] = _u[0] / _dist_real;
	_B->M[0][1] = _u[1] / _dist_real;
	_B->M[0][2] = _u[2] / _dist_real;
	_B->M[1][0] = _n[0];
	_B->M[1][1] = _n[1];
	_B->M[1][2] = _n[2];
	

	r0 = POLY_O2( _dist_real -_dist, 0.00, K_e, 0.00);
	
	if(_B->id() >= 0)
	{
		_B->F += -1.0 * _u * r0;
	}

	if(_A->id() >= 0)
	{
		_A->F += 1.0 * _u * r0;
	}
	
}

double Edge::distance_to( Vector pos ) { 

	double r0, r1, r2;
	
	if( _A == NULL || _B == NULL )
		return 0.0;

	_update_vars();

	_v = pos - X;

	_u = orth(_v, _n);
	
	r0 = norm( _u );

	if( r0 / _scale > 1.0 ) {

		_v = pos - _A->X;
		_u = pos - _B->X;

		r1 = norm( _v );
		r2 = norm( _u );

		if( r1 > r2 )
			return r2;
		
		return r1;

	}

	return norm( proj( _v, _n ) ); 


}

void Edge::collid_with(SpatialObject *p) {

	Vector s;
	double r0, r1, R, r2;
	
	if( _A == NULL || _B == NULL || p == _A || p == _B)
		return;

	_update_vars();

	_u = p->X - X;

	s = proj(_u, _n);

	r0 = norm(s);

	if( r0 > 0.0 )
		s /= r0;
	
	R = inner_prod( s, p->dX );
	r1 = p->distance_to ( X + orth(_u, _n) );

	if( R < 0.0 || r1 < 0.0)
	{
		
		double a;
		r2 = norm( proj(_u, _n) );
		a = -900 / ((r2-r1)*(r2-r1)) * 11.589;

		r1 = POLY_O2( 1.00 - (r2-r1)*(r2-r1)/(r2*r2), 0.00, a, 0.00);

		p->F += s * r1 * r0 ;
		
	}
	
}
