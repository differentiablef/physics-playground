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

#ifndef _MODEL_H_
#define _MODEL_H_

#include "common.h"
#include "spatial-object.h"
#include "particle.h"
#include "edge.h"


class Model {
	public:
		Model() {
			_particles = NULL;
			_edges = NULL; 
			_particles_len = _edges_len = 0;
		};

		virtual ~Model() {
			if(_particles) 
				delete [] _particles;
			
			if(_edges)
				delete [] _edges;
		};

	public:

		void copy( Model &in );
		
		int size() { return _particles_len + _edges_len; };
		
		SpatialObject *operator[](int i) {
			if( i < _particles_len )
				return &_particles[i];

			return &_edges[ i - _particles_len ];
		}

		Particle *get_particle_by_id(int id)
			{ return _id_map[id]; };

		Particle *get_particles() { return _particles; };
		Edge *get_edges() { return _edges; };

		void set_particles( Particle *pt, int plen ) {
			_particles = pt;
			_particles_len = plen;
			
			for( int i = 0; i < plen; i++ )
				_id_map[ _particles[i].id() ] = &_particles[i]; 
		};
		
		void set_edges( Edge *pt, int plen )
			{ _edges = pt; _edges_len = plen; };

		int edge_count() { return _edges_len; };
		int particle_count() { return _particles_len; };

	protected:
		 
		std::map<int, Particle*> _id_map;
		
		Particle *_particles;
		int _particles_len;

		Edge *_edges;
		int _edges_len;

};


// /////////////////////////////////////////////////////////////////////////////
// Helper Functions

void read_state( const char *filename, Model &md );
void dump_state( Model &md, double time = -1.00 );

#endif // _MODEL_H_

