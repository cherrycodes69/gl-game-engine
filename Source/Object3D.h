#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <vector>
#include <GLAUX.H>
#include "TextureCollection.h"
#include "Vector3D.h"
#include <algorithm>
#include <GLU.H>
#include <list>
using namespace::std;


class Container3D;

class Object3D
{
public:

	Vector3D pos;
	Vector3D dim;
	Vector3D dir;

	GLfloat	xrot;				
	GLfloat	yrot;				
	
	Container3D *super;

	double bindingRadius;
	float speed;
	double gravity;

	Object3D(){};
	Object3D(double _x,double _y,double _z,double _l,double _h,double _d,GLfloat _xrot=0,GLfloat _yrot=0)
		:pos(_x,_y,_z),dim(_l,_h,_d),xrot(_xrot),yrot(_yrot),dir(0,0,0),bindingRadius(_l/2),gravity(1),speed(0.1){dir.normalize();}
	
	virtual void update();
	virtual void draw();
	virtual void calculateNewDirection(Object3D& b);//make virtual for all objects
	virtual Vector3D center(){return pos+(dim*0.5);}
	virtual bool intersects(Object3D& o);


	void handleCollisions();//move to all objects
	void forceInBounds();//move to all objects
	void stepForward();//move to all objects
	void stepBack();//move to all objects
	bool converging(Object3D& b);//move to all objects
	void forceNegative(double &f);
	void forcePositive(double &f);
	void applyGravity();
	
	
};


class Sphere3D: public Object3D
{

	public:
	TextureCollection TX;
	GLUquadricObj *quadratic;
	GLfloat r,g,b;
	int rotspeed;

	Sphere3D(){super=0;}
	Sphere3D(double radius, Container3D* containingField,TextureCollection _TX ,GLUquadricObj *_quadratic,
			 double _dx,double _dy,double _dz,double _x,double _y,double _z,int _rotspeed,double _speed=1);
	
	void update();	
	bool intersects(Object3D& b);

	void calculateNewDirection(Object3D& b);
	void draw();
	Vector3D center(){return pos;}


};



class Container3D:public Object3D
{
	public:
	list<Object3D*> objects;

	bool draw_borders;
	Container3D(){};
	Container3D(double _x,double _y,double _z,double _l,double _h,double _d,Container3D *_super=0, bool borders=false):Object3D(_x,_y,_z,_l,_h,_d){
		super=_super;
		draw_borders=borders;
		
	};
	void addObject(Object3D* o);
	

	void update();//make virtual for all objects
	void draw();//make virtual for all objects
	void drawBorders();
	bool containsOriginOf(const Object3D& o){
		if(pos<o.pos && o.pos<pos+(dim)){return true;}return false;};
	~Container3D();
	//bool contains( Object3D& o){if( ( (o.center()+(o.dim*0.5)>center()-(dim*0.5)) && o.center()<center() ) || ( (o.center()-(o.dim*0.5)<center()+(dim*0.5)) && o.center()>center() ) )
};
class TopContainer:public Container3D
{
	public:
	vector<Container3D> sectors;
	int n;
	double lStep;
	double hStep;
	double dStep;
	TopContainer(int _n,double l,double h, double d,bool db);
	TopContainer(){};
	
	void update();//make virtual for all objects
	Container3D& findSectorFor(Object3D& c);
	void draw();//make virtual for all objects
	

};

bool operator==(const Object3D& a,const Object3D& b);


#endif