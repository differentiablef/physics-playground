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

#include <iostream>


#include "common.h"
#include "display-area.h"

#include "particle.h"
#include "edge.h"

#define SPATIALOBJ_DRAW(obj,cr) \
{\
	(cr)->translate( (obj)->x, (obj)->y );\
	(obj)->draw( (cr) );\
	(cr)->translate( -1.00* (obj)->x, -1.00*(obj)->y );\
}
	


bool DisplayArea::on_draw (const Cairo::RefPtr< Cairo::Context> &cr)
{
	Gtk::Allocation aloc = get_allocation();
	
	int width, height;

	width  = aloc.get_width ();
	height = aloc.get_height();

	cr->save();

	cr->scale(width/_w, height/_h);
	cr->rotate_degrees (180);
	cr->translate( _ux, -1.0*_uy );
	cr->scale(-1.0, 1.0);
	cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);

	if( this->disp_objs.size()  > 0 ) {

		SpatialObject *pt = NULL;

		for( int i = 0; i < this->disp_objs.size(); i++ ) {
			
			pt = this->disp_objs[i];
			
			cr->translate ( pt->X[0], pt->X[1] );
			
			if( !( pt->draw(cr) ) ) {
				MARKER("Error: Failed to draw SpatialObject");
			}
			cr->translate ( -1.0 * pt->X[0], -1.0 * pt->X[1] );
			
		}

	} else {
		MARKER("No Objects To Draw");
	}

	cr->restore();
	
	return true;
	
}


