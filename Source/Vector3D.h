#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <math.h>
class Vector3D
{
	public:
	double dx;
	double dy;
	double dz;

	Vector3D(){};
	Vector3D(double _dx,double _dy,double _dz):dx(_dx),dy(_dy),dz(_dz){};
	Vector3D(const Vector3D &other):dx(other.dx),dy(other.dy),dz(other.dz){};

	double getMag();
	double dot(const Vector3D &other);
	Vector3D subtract(const Vector3D &other)const{
		Vector3D r(*this);
		r.dx-=other.dx;r.dy-=other.dy;r.dz-=other.dz;
		return r;}
	Vector3D add(const Vector3D &other)const{
		Vector3D r(*this);
		r.dx+=other.dx;r.dy+=other.dy;r.dz+=other.dz;
		return r;}
	Vector3D multiply(double f)const;
	Vector3D invert();
	Vector3D normalize();

	Vector3D operator-(const Vector3D &o)const{return subtract(o);}
	Vector3D operator+(const Vector3D &o)const{return add(o);}
	Vector3D operator*(double f)const{return multiply(f);}
	bool operator<(const Vector3D& b)const;
	bool operator>(const Vector3D& b)const;

};
bool operator==(const Vector3D& a,const Vector3D& b);
#endif