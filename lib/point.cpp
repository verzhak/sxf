
#include "lib/point.hpp"

CVertex::CVertex() :
	x(0), y(0), z(0)
{
	;
}

CVertex::CVertex(const CVertex & v) :
	x(v.x), y(v.y), z(v.z)
{
	;
}

CVertex::CVertex(const double t_x, const double t_y, const double t_z)
{
	x = t_x;
	y = t_y;
	z = t_z;
}

const CVertex & CVertex::operator=(const CVertex & v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return * this;
}

double CVertex::operator[](const unsigned ind) const
{
	switch(ind)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
	}

	throw_;
}

double & CVertex::operator[](const unsigned ind)
{
	switch(ind)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
	}

	throw_;
}

const CVertex CVertex::operator+(const CVertex & v)
{
	CVertex ret;

	ret.x = x + v.x;
	ret.y = y + v.y;
	ret.z = z + v.z;

	return ret;
}

const CVertex & CVertex::operator+=(const CVertex & v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return * this;
}

const CVertex CVertex::operator-(const CVertex & v)
{
	return CVertex(x - v.x, y - v.y, z - v.z);
}

double CVertex::euclidean_distance_to(const CVertex & op) const
{
	return sqrt(pow(x - op.x, 2) + pow(y - op.y, 2) + pow(z - op.z, 2));
}

