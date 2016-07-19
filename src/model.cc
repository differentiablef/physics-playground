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

#include "model.h"

#include <fstream>


void Model::copy(Model &in) {
	register int i, k;
	Particle *pt1,*pt2;
	
	if( _particles == NULL && _edges == NULL ) {

		_particles     = new Particle[ in._particles_len ];
		_edges         = new Edge[ in._edges_len ];
		_particles_len = in._particles_len;
		_edges_len     = in._edges_len;
		
	} else {
		
		if( in._edges_len != _edges_len ) {
			
			if( _edges != NULL )
				delete [] _edges;

			_edges = new Edge[ in._edges_len ];
			_edges_len = in._edges_len;

		}

		if( in._particles_len != _particles_len ) {
			
			if( _particles != NULL )
				delete [] _particles;

			_particles = new Particle[ in._particles_len ];
			_particles_len = in._particles_len;
		}

	}

	for( i = 0; i < in._particles_len; i++ ) {
		
		_particles[ i ] = in._particles[ i ];

		_id_map[ _particles[ i ].id() ] = &(_particles[i]); 
		
	}

	for( i = 0; i < in._edges_len; i++ ) {

		_edges[i] = in._edges[i];

		pt1 = (Particle*)in._edges[i].A();
		pt2 = (Particle*)in._edges[i].B();

		if( pt1 != NULL )
			_edges[i].set_A( _id_map[ pt1->id() ] );
		
		if( pt2 != NULL )
			_edges[i].set_B( _id_map[ pt2->id() ] );
		
	}
	
}

// /////////////////////////////////////////////////////////////////////////////
// Helper Functions


void dump_state( Model &md, double time )
{
	std::string out;
	
	for( int i = 0; i < md.particle_count (); i++ ) {
		std::cout << md[i]->to_string();
		if( time >= 0.0 )
			std::cout << "\t" << time;
		std::cout << "\n";
	}

	std::cout << out << std::endl;
}


void read_state( const char *filename, Model &md ) 
{
	std::string str;
	std::ifstream ifs;
	
	std::map<int, Particle> pts;
	std::map<int, Particle>::iterator it_pts; 
	
	std::map<int, std::map<int, double> > edg;
	std::map<int, std::map<int, double> >::iterator it_A;
	std::map<int, double>::iterator it_B;

	std::map<int, std::map<int, double> > sedg;

	Particle ptmp;
	Particle *pt;
	Edge *ed;
	int plen, elen, i;

	ifs.open( filename, std::ifstream::in );

	if( !ifs.is_open() ) {
		std::cerr << "Error: Cannot open file " << filename << std::endl;
		return;
	}

	elen = 0;

	while(ifs.good()) {
		ifs >> str;

		if( str == PARTICLE_TYPE ) {

			ifs >> ptmp.id() >> ptmp.radius()
				>> ptmp.X[0]  >> ptmp.X[1]  >> ptmp.X[2]
				>> ptmp.dX[0] >> ptmp.dX[1] >> ptmp.dX[2]
				>> ptmp.F[0] >> ptmp.F[1] >> ptmp.F[2];

			pts[ ptmp.id() ] = ptmp;
			
		} else if( str == EDGE_TYPE ) {
			
			int id_A, id_B;
			double ed_length;
			
			ifs >> ed_length >> id_A >> id_B;

			edg[id_A][id_B] = ed_length;

			elen++;
			
		}

		str = "";
		
	}

	ifs.close();

	plen = pts.size();
	pt   = new Particle[ plen ];
	
	ed   = new Edge[ elen ];

	
	i = 0;
	
	for( it_pts = pts.begin(); it_pts != pts.end(); it_pts++ )
	{
		pt[i] = it_pts->second;
		i++;
	}

	md.set_particles( pt, plen );

	i = 0;
	
	for( it_A = edg.begin(); it_A != edg.end(); it_A++ )
	{
		for( it_B = it_A->second.begin(); it_B != it_A->second.end(); it_B++ )
		{
			ed[i].set_length( it_B->second );
			ed[i].set_A( md.get_particle_by_id( it_A->first ) );
			ed[i].set_B( md.get_particle_by_id( it_B->first ) );
			i++;

		}
	}

	DEBUG(elen);

	md.set_edges( ed, elen );


	return;
	
}




