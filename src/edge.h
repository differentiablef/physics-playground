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

#ifndef _EDGE_H_
#define _EDGE_H_

#include <gtkmm.h>

#include "common.h"
#include "spatial-object.h"

#define EDGE_TYPE   "EDGE"

class Edge : public SpatialObject {
	public:
		Edge() : SpatialObject() 
			{ _A  = _B = NULL; _dist = _dist_real = 0.0; 
			 _type = EDGE_TYPE; };
		
		virtual ~Edge() {};

		SpatialObject *A() { return _A; }
		SpatialObject *B() { return _B; }

		void set_A(SpatialObject *pt)
			{ _A = pt; _update_vars(); };

		void set_B(SpatialObject *pt)
			{ _B = pt; _update_vars(); };

		double &length() { return _dist; };
		
		void set_length( double r )
			{ _dist = r; }
		
	public: // Overloads
		virtual std::string to_string();
		
		virtual bool draw( const Cairo::RefPtr< Cairo::Context > &cr );
		virtual void update_forces();
		
		virtual double distance_to( Vector pos );
		virtual void collid_with(SpatialObject *p);
		
	protected:

		void _update_vars();
		
		SpatialObject *_A, *_B;
		
		double _scale, _dist, _dist_real;
		
	protected:
		Vector _u, _n, _v;
};

#endif // _EDGE_H_

