#ifndef OBJECT2D_H
#define OBJECT2D_H
#include <vector>
#include <math.h>
//#include <gl\GLAUX.H>
#include "TextureCollection.h"
#include <algorithm>
#include <gl\gl.h>	
#include <gl\glu.h>			

using namespace::std;


class Vector2D
{
	public:
	double dx;
	double dy;


	Vector2D(){};
	Vector2D(double _dx,double _dy):dx(_dx),dy(_dy){};
	Vector2D(const Vector2D &other):dx(other.dx),dy(other.dy){};
	double getMag();
	double dot(const Vector2D &other);
	Vector2D subtract(const Vector2D &other){
		Vector2D r(*this);
		r.dx-=other.dx;r.dy-=other.dy;
		return r;}
	Vector2D add(const Vector2D &other){
		Vector2D r(*this);
		r.dx+=other.dx;r.dy+=other.dy;
		return r;}
	Vector2D multiply(double f);
	Vector2D invert();
	Vector2D normalize();
};



class Container2D;

class Object2D
{
public:
	double x,y;
	double l,h;
	Vector2D dir;
	Container2D *super;
	double bindingRadius;
	float speed;
	

	Object2D(){};
	Object2D(double _x,double _y,double _l,double _h):x(_x),y(_y),l(_l),h(_h){};
	virtual void update(){};
	
	
};


class Circle2D: public Object2D
{

	public:
	TextureCollection TX;

	GLfloat r,g,b;

	GLUquadricObj *quadratic;
	bool dynamic;
	bool up;
	bool right;
	bool collisionHandled;
	HANDLE thread;

	Circle2D(){super=0;}

	Circle2D(double radius, Container2D* containingField,TextureCollection _TX,double _dx,double _dy,double _x,double _y);
	
	void update();
	void forceNegative(double &f);
	void forcePositive(double &f);
	void calculateMovements();
	bool intersects(const Object2D& b);
	void calculateNewDirection(Circle2D& b);
	void forceInBounds();
	void stepForward();
	void stepBack();
	void handleCollisions();
	void draw();
	bool converging(const Object2D& b);




};


class Container2D:public Object2D
{
	public:
	vector<Circle2D> objects;

	Container2D(){};
	Container2D(double _x,double _y,double _l,double _h,Container2D *_super=0):Object2D(_x,_y,_l,_h){
		super=_super;
 
	};
	void addObject(Circle2D o);
	void update();
	void draw();
	void clear(){};
	bool contains(const Circle2D& o){if(o.x>=x && o.x<=x+l && o.y>=y && o.y<=y+h){return true;}return false;};
};
class TopContainer:public Container2D
{
public:
	vector<Container2D> sectors;
	const static int len=42;
	const static int height=35;
	TopContainer(int n)
	{
		float vStep=height/n;
		float hStep=len/n;
		float originX=0-len/2;
		float originY=0-height/2;
		for(int i=0;hStep*i<len;++i)
			for(int j=0;vStep*j<height;++j)
					sectors.push_back(Container2D(originX+hStep*i,originY+vStep*j,vStep,hStep,this));
		//sectors.push_back(Container2D(originX+hStep*i-hStep/8,originY+vStep*j-vStep/8,hStep*1.25,vStep*1.25));
		
		
	};
	
	void update();
	Container2D& findSectorFor(Circle2D& c);
	void draw();
	

};
bool operator==(const Vector2D& a,const Vector2D& b);
bool operator==(const Object2D& a,const Object2D& b);

#endif