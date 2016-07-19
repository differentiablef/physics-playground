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

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <gtkmm.h>
#include <vector>
#include <string>

#include <gtkmm/drawingarea.h>

#include "spatial-object.h"

#define PARTICLE_TYPE   "PARTICLE"

class Particle : public SpatialObject
{
	public:
		Particle() : SpatialObject() { _type = PARTICLE_TYPE; };
		virtual ~Particle() {};

	public:
		
		double &radius()
		   { return _R; };

	public:

		virtual std::string to_string();
		
		virtual bool draw( const Cairo::RefPtr< Cairo::Context > &cr );

		// return the signed distance between the particle and p
		virtual double distance_to( Vector pos );

		// Update the forces acting on the particle to account for a collision
		// with sp
		virtual void collid_with(SpatialObject *p);
		
		virtual void update_forces();

	protected:

		double _R;

};

#endif // _PARTICLE_H_

