#include "Vector3D.h"
double Vector3D::getMag ()
{
	return sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));
}

double Vector3D::dot(const Vector3D &other)
{
	return dx*other.dx+dy*other.dy+dz*other.dz;
}

Vector3D Vector3D::multiply(double f)const
{
	Vector3D r(*this);
	r.dx*=f;
	r.dy*=f;
	r.dz*=f;
	return r;
}

Vector3D Vector3D::invert()
{
	Vector3D r(*this);
	r.dx*=-1;
	r.dy*=-1;
	r.dz*=-1;
	return r;
}

Vector3D Vector3D::normalize()
{
	Vector3D r(*this);
	double len=getMag();
	r.dy/=len;
	r.dx/=len;
	r.dz/=len;
	return r;
}
bool operator==(const Vector3D& a,const Vector3D& b)
{
	if(a.dx==b.dx && a.dy==b.dy && a.dz==b.dz)
		return true;
	return false;
}
bool Vector3D::operator<(const Vector3D& b)const
{
	if(dx<b.dx && dy<b.dy && dz<b.dz)
		return true;
	return false;
}
bool Vector3D::operator>(const Vector3D& b)const
{
	if(dx>b.dx && dy>b.dy && dz>b.dz)
		return true;
	return false;
}