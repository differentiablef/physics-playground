/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc

 * Copyright (C) 2013 Michael Blackmon <burned@pluto>
 * 
 */

#include <gtkmm.h>
#include <cstdlib>
#include <vector>
#include <map>

#include <fstream>
#include <iostream>
#include <string>

#include "config.h"

#include "common.h"
#include "math-common.h"
#include "display-area.h"
#include "spatial-object.h"
#include "particle.h"
#include "edge.h"
#include "model.h"

#define PARTICLE_COUNT     100

void update_vector_field(Model &mv) {
	register int i, k;
	Particle *pt, *pp1;
	Edge *ed;
	Vector f;
	int collisions = 0;

	
	std::map< double, std::map< double, Particle * > > pt_map;
	std::map< double, std::map< double, Particle * > >::iterator it_x, it_xL, it_xR;
	std::map< double, Particle * >::iterator it_yL, it_yU, it_y;
	
	double r1;

	pt = mv.get_particles();
	ed = mv.get_edges();

	for( i = 0; i < mv.particle_count(); i++ )
	{
		pt[i].clear_forces ();
		
		if( pt[i].id() >= 0 )
		{
			pt_map[ pt[i].X[0] ][ pt[i].X[1] ] = &pt[i];

			f[0] =  00.00 + 2.00 * pt[i].X[1];
			f[1] = -1.00 - 2.00 * pt[i].X[0];

			pt[i].F += f;
		}
	}

	
	for( i = 0; i < mv.size(); i++ )
		mv[i]->update_forces ();
	
	
	for( i = 0; i < mv.particle_count(); i++ )
	{
		if( pt[i].id() >= 0 )
		{
		
			it_xL = pt_map.lower_bound( pt[i].X[0] - 2.0 * pt[i].radius() );
			it_xR = pt_map.upper_bound( pt[i].X[0] + 2.0 * pt[i].radius() );

			for( it_x = it_xL; it_x != it_xR; it_x++ )
			{

				it_yL = it_x->second.lower_bound( pt[i].X[1] - 2.0 * pt[i].radius () );
				it_yU = it_x->second.upper_bound( pt[i].X[1] + 2.0 * pt[i].radius () );

				for( it_y = it_yL; it_y != it_yU; it_y++ )
				{
					pp1 = it_y->second;

					r1 = pt[i].distance_to (pp1->X);

					if( pp1->id() < pt[i].id() && r1 < pp1->radius() )
					{
						pt[i].collid_with( pp1 );
						pp1->collid_with ( &pt[i] );
					}
					

				}

			}
			
		}

			
		// 8*N_p*(Ln(N_p))^2
		
	}
	

	for( i = 0; i < mv.edge_count(); i++ )
	{ 
		
		if( ed[i].A()->id() < 0 && ed[i].B()->id() < 0 )
		{
			for( k = 0; k < mv.particle_count (); k++ ) {

				r1 = ed[i].distance_to(pt[k].X);
				if( r1 < pt[k].radius() )
					ed[i].collid_with (&pt[k]);
				
			}
		}
		
	}
	
}

static double current_time = 0.0;

void iterate_model(Model &sy) {
	register int i;
    Particle *pt;
	Edge *con_eds;

	Vector Mo1, Mo2, A;
	double Ke1, Ke2;
	
	static Model m1, m2, m3, m4;


	for( i = 0; i < sy.particle_count (); i++ ) {
		
		Mo1 += 1.0 * sy[i]->mass * sy[i]->dX;
		Ke1 += 0.5 * sy[i]->mass * norm2( sy[i]->dX );
		
	}

	
	m1.copy( sy );

	update_vector_field( m1 );
	
	m2.copy( m1 );
	
	for( i = 0; i < m2.particle_count(); i++ ) {
		
		m2[i]->dX  = sy[i]->dX;
		m2[i]->dX += (0.5 * DT * (m1[i]->F));
		
		m2[i]->X  = sy[i]->X;
		m2[i]->X += (0.5 * DT * (m1[i]->dX));
		
	}

	update_vector_field( m2 );
	
	m3.copy( m2 );
	
	for( i = 0; i < m3.particle_count(); i++ ) {
		
		m3[i]->dX  = sy[i]->dX;
		m3[i]->dX += (0.5 * DT * (m2[i]->F));
		
		m3[i]->X  = sy[i]->X;
		m3[i]->X += (0.5 * DT * (m2[i]->dX));
		

	}

	update_vector_field( m3 );

	m4.copy( m3 );
	
	for( i = 0; i < m4.particle_count(); i++ ) {

		m4[i]->dX  = sy[i]->dX;
		m4[i]->dX += (DT * (m3[i]->F));
		
		m4[i]->X  = sy[i]->X;
		m4[i]->X += (DT * (m3[i]->dX));
		
	}

	update_vector_field( m4 );

	// Update the velocity and position of each particle in the model
	
	for( i = 0; i < sy.particle_count (); i++ ) {

		sy[i]->dX += 0.1666666667 * DT * 
			(m1[i]->F + 2.0*m2[i]->F + 2.0*m3[i]->F + m4[i]->F);
		
		sy[i]->X  += 0.1666666667 * DT * 
			( m1[i]->dX + 2.0*m2[i]->dX + 2.0*m3[i]->dX + m4[i]->dX);
		
	}

	update_vector_field( sy );

	for( i = 0; i < sy.particle_count (); i++ ) {
		
		Mo2 += 1.0 * sy[i]->mass * sy[i]->dX;
		Ke2 += 0.5 * sy[i]->mass * norm2( sy[i]->dX );
		
	}

	
	MARKER("Energy and Momentum");
	DEBUG(Ke2);
	DEBUG(Mo2[0]);
	DEBUG(Mo2[1]);
	DEBUG(Mo2[2]);
	
	MARKER("Difference:");
	DEBUG(Ke1 - Ke2);
	DEBUG(Mo1[0] - Mo2[0]);
	DEBUG(Mo1[1] - Mo2[1]);
	DEBUG(Mo1[2] - Mo2[2]);
	

	current_time += DT;
	
	//dump_state (sy, current_time);
}


// /////////////////////////////////////////////////////////////////////////////
// Section: Main and other specific implementation details

static Model model;
static DisplayArea *widg;


bool update_model(DisplayArea *ptr) {
	
	iterate_model ( model );
	ptr->queue_draw();
	return true;
}

void generate_grid(double ux, double uy, double width, double height, int N, int M, bool edges = false)
{
	double dx = 0.90*width/N, dy = 0.9*height/M, r, x0, y0;
	Particle pt[N][M], p1, p2, p3, p4;
	Edge edg;
	int id = 0, i, k;

	x0 = ux + 0.05*width;
	y0 = uy - 0.05*height;
	
	r = pow( dx*dx + dy*dy, 0.5 )/4.0;

	p1.X[0] = ux;
	p1.X[1] = uy;
	p1.radius() = 0.0001;
	p1.id() = -1;

	std::cout << p1.to_string() << std::endl;
	
	p2.X[0] = ux + width;
	p2.X[1] = uy;
	p2.radius() = 0.0001;
	p2.id() = -2;
	
	std::cout << p2.to_string() << std::endl;

	p3.X[0] = ux + width;
	p3.X[1] = uy - height;
	p3.radius() = 0.0001;
	p3.id() = -3;
	
	std::cout << p3.to_string() << std::endl;
	
	p4.X[0] = ux;
	p4.X[1] = uy - height;
	p4.radius() = 0.0001;
	p4.id() = -4;

	std::cout << p4.to_string() << std::endl;
	
	for( i = 0; i < N; i++ ) {
		for( k = 0; k < M; k++ ) {
			
			pt[i][k].X[0]      = x0 + i * dx;
			pt[i][k].X[1]      = y0 - k * dy;

			pt[i][k].dX[0]     = ((random() % 100) - 50.0)/25.0;
			pt[i][k].dX[1]     = ((random() % 100) - 50.0)/25.0;
			
			pt[i][k].radius() = r;
			pt[i][k].id()     = id++;

			std::cout << pt[i][k].to_string () << std::endl;
			
		}
		
	}

	r *= 2.1;
	if(edges) {

		for( i = 0; i < N; i++ )
		{
			for( k = 0; k < M; k++ ) 
			{

				if( k < M - 1 )
				{
					edg.set_length( r );

					edg.set_A( &pt[i+0][k+0] );
					edg.set_B( &pt[i+0][k+1] );

					std::cout << edg.to_string () << std::endl;
				}

				if( i < N - 1 )
				{
					edg.set_length( r );

					edg.set_A( &pt[i+0][k+0] );
					edg.set_B( &pt[i+1][k+0] );

					std::cout << edg.to_string () << std::endl;
				}

				if( i < N - 1 && k < M - 1 )
				{
					edg.set_length ( sqrt(2) * r );

					edg.set_A( &pt[i+0][k+0] );
					edg.set_B( &pt[i+1][k+1] );

					std::cout << edg.to_string () << std::endl;
				}

			}

		}
		
	}


	edg.set_A (&p1);
	edg.set_B (&p2);
	edg.set_length (width);
	
	std::cout << edg.to_string () << std::endl;
	
	edg.set_A (&p2);
	edg.set_B (&p3);
	edg.set_length (height);
	
	std::cout << edg.to_string () << std::endl;
	
	edg.set_A (&p3);
	edg.set_B (&p4);
	edg.set_length (width);
	
	std::cout << edg.to_string () << std::endl;

	edg.set_A (&p4);
	edg.set_B (&p1);
	edg.set_length (height);
	
	std::cout << edg.to_string () << std::endl;
}

int run_simulation(int argc, char *argv[], Model &md) 
{

	Gtk::Main kit(argc, argv);
	Gtk::Window win(Gtk::WINDOW_TOPLEVEL);

	DisplayArea disp(UPPER_X-R_c, UPPER_Y+R_c, WIDTH + 2.0*R_c, HEIGHT+2.0*R_c);
	
	sigc::slot<bool> time_iter;
	sigc::connection time_conn;


	// Add all the elements to be drawn.
	for( int i = 0; i < md.size(); i++ )
		disp.add (md[i]);
	
	// Setup timeout based model iterator 
	time_iter = sigc::bind( sigc::ptr_fun(update_model), &disp ); 
	time_conn = Glib::signal_timeout().connect( time_iter, 10 );

	win.set_title ("Display");
	win.set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT);
	win.add( disp );
	disp.show();

	kit.run(win);

	return 0;
}

int
main (int argc, char *argv[])
{

	//generate_grid ( UPPER_X, UPPER_Y, WIDTH, HEIGHT, 10, 10, false );

	if( argc > 1 )
		read_state (argv[1], model);
	else
		read_state ("data/initial_grid_pendulum.dat", model);

	// Run and Display the simulation

	/*
	while(!0) 
	{
		iterate_model ( model );
		DEBUG( current_time );
	}
*/
	
	
	return run_simulation (argc, argv, model);
	
}

