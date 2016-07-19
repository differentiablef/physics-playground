
#ifndef _MATH_COMMON_H_
#define _MATH_COMMON_H_

#include <math.h>
#include <sstream>

// /////////////////////////////////////////////////////////////////////////////
// Basic Vector definition, prototypes, and macros.

class Vector {
	public:
		Vector() { C[0] = C[1] = C[2] = 0.0; };
		Vector(double x, double y, double z) { C[0] = x; C[1] = y; C[2] = z; };
		
	public:
		double C[3];

		double &operator[](int i)
			{ return C[i]; };

		Vector &operator+=(const Vector &vt)
		    { C[0] += vt.C[0]; C[1] += vt.C[1]; C[2] += vt.C[2];
			  return *this; };

		Vector &operator-=(const Vector &vt)
		    { C[0] -= vt.C[0]; C[1] -= vt.C[1]; C[2] -= vt.C[2];
			  return *this; };

		Vector &operator*=(const double db)
		    { C[0] *= db; C[1] *= db; C[2] *= db;
			  return *this; };
		
		Vector &operator/=(const double db)
		    { C[0] /= db; C[1] /= db; C[2] /= db;
			  return *this; };

		Vector &operator=(const Vector &vt)
			{ C[0] = vt.C[0]; C[1] = vt.C[1]; C[2] = vt.C[2];
			  return *this; };

		Vector operator +(const Vector &v2)
		    { Vector tmp = *this;
			  tmp+=v2;
			  return tmp; };

		
		Vector operator -(const Vector &v2)
			{ Vector tmp = *this;
			  tmp-=v2;
			  return tmp; };
		
		Vector operator *(const double &db)
			{ Vector tmp = *this;
			  tmp*=db;
			  return tmp; };
		
		Vector operator /(const double db)
			{ Vector tmp = *this;
			  tmp/=db;
			  return tmp; };


		std::string to_string() 
		{
			std::stringstream ss;
			ss << C[0] << '\t' << C[1] << '\t' << C[2];
			return ss.str();
		};
};

Vector operator +(const Vector &v1, const Vector &v2);
Vector operator -(const Vector &v1, const Vector &v2);
Vector operator *(const double &db,  const Vector &v2);
Vector operator /(const Vector &v1, const double db);


Vector proj( const Vector &v1, const Vector &u1 );
Vector orth( const Vector &v1, const Vector &u1 );

double norm2( const Vector &v1 );
double norm( const Vector &v1 );
double inner_prod( const Vector &v1, const Vector &u1 );


#define NORM2(v)             ((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2])
#define NORM(v)              (pow( NORM2((v)), 0.5 ))

#define INNER_PROD(u,v)	     ((u)[0] * (v)[0] + (u)[1] * (v)[1] + (u)[2] * (v)[2])



// /////////////////////////////////////////////////////////////////////////////
// Quick Polynomial Macros

#define POLY_O4(x,a,b,c,d,e)   ((e) + (x) * ((d) + (x) * ((c) + (x) * ((b) + (x) * (a)))))
#define POLY_O3(x,a,b,c,d)     ((d) + (x) * ((c) + (x) * ((b) + (x) * (a))))
#define POLY_O2(x,a,b,c)       ((c) + (x) * ((b) + (x) * (a)))

#define INT_POLY_04(x,a,b,c,d,e)   (x) * ((e) + (x) * ((d/2.0) + (x) * ((c/3.0) + (x) * ((b/4.0) + (x) * (a/5.0)))))
#define INT_POLY_O3(x,a,b,c,d)     (x) * ((d) + (x) * ((c/2.0) + (x) * ((b/3.0) + (x) * (a/4.0))))
#define INT_POLY_O2(x,a,b,c)       (x) * ((c) + (x) * ((b/2.0) + (x) * (a/3.0)))


#endif // _MATH_COMMON_H_

