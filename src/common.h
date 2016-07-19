
#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <math.h>

#ifndef DEBUG
#define DEBUG(x) \
	std::cerr << "DEBUG: "#x" == " << (x) << std::endl;
#endif

#ifndef MARKER
#define MARKER(x) \
	std::cerr << "MARKER: " << (x) << std::endl;
#endif

#define DT	          0.0005
#define DELTA		  0.05

#define RADIUS        0.05
#define R_c           0.1

#define K_c			  100.0 
#define K_cd          2.0*sqrt(K_c)

#define K_e           100.0
#define K_ed          2.0*sqrt(K_e)

#define K_b           1.00
#define K_f           2.0
#define G             0.0

#define WIDTH         3.0
#define HEIGHT        3.0
#define UPPER_X      -1.5
#define UPPER_Y       1.5

#define WINDOW_HEIGHT 400
#define WINDOW_WIDTH  400



#endif
