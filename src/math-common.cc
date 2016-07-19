
#include "math-common.h"


Vector operator +(const Vector &v1, const Vector &v2)
{
	return Vector( v1.C[0] + v2.C[0], v1.C[1] + v2.C[1], v1.C[2] + v2.C[2] );
}

Vector operator -(const Vector &v1, const Vector &v2)
{
	return Vector( v1.C[0] - v2.C[0], v1.C[1] - v2.C[1], v1.C[2] - v2.C[2] );
}


Vector operator *(const double &db,  const Vector &v2)
{
	return Vector( db * v2.C[0], db * v2.C[1], db * v2.C[2] );
}


Vector operator /(const Vector &v1, const double db)
{
	return Vector( v1.C[0] / db, v1.C[1] / db, v1.C[2] / db );
}


double norm2( const Vector &v1 )
{
	return v1.C[0] * v1.C[0] + v1.C[1] * v1.C[1] + v1.C[2] * v1.C[2];
}


double norm( const Vector &v1 )
{
	return pow( v1.C[0] * v1.C[0] + v1.C[1] * v1.C[1] + v1.C[2] * v1.C[2], 0.5 );
}


double inner_prod( const Vector &v1, const Vector &u1 )
{
	return v1.C[0] * u1.C[0] + v1.C[1] * u1.C[1] + v1.C[2] * u1.C[2];
}

Vector proj( const Vector &v1, const Vector &u1 )
{
	double r0 = inner_prod (v1, u1) / norm2( u1 ); 

	return r0 * u1;
}


Vector orth( const Vector &v1, const Vector &u1 )
{
	double r0 = inner_prod (v1, u1) / norm2( u1 );
	
	return v1 - r0 * u1;
	
}



