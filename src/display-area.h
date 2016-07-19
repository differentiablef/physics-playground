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

#ifndef _DISPLAY_AREA_H_
#define _DISPLAY_AREA_H_


#include <vector>
#include <gtkmm/drawingarea.h>

#include "spatial-object.h"

class DisplayArea: public Gtk::DrawingArea 
{
	public:

		DisplayArea() 
	       { _ux = -1.0; _uy = 1.0; _w = 2.0; _h = 2.0; };
		
		// constructor with upper left corner, width and height arguments
		DisplayArea(double ux, double uy, double w, double h) 
	       { _ux = ux; _uy = uy; _w = w; _h = h; };
		
		virtual ~DisplayArea() {};

	public:
		void add( SpatialObject *sp )
	         { this->disp_objs.push_back(sp); };

		SpatialObject *operator[](unsigned int i)
	         { return this->disp_objs[i]; };

		int object_count() { return disp_objs.size(); };

	protected:
		virtual bool on_draw (const Cairo::RefPtr< Cairo::Context> &cr);
	

	private:
		double _ux, _uy, _w, _h;
		std::vector<SpatialObject*> disp_objs;

};

#endif // _DISPLAY_AREA_H_

