#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <vector>
#include <GLAUX.H>
#include "TextureCollection.h"
#include "Vector3D.h"
#include <algorithm>
#include <GLU.H>
#include <list>
#include <string.h>
using namespace::std;


class Space3D;
class Player;
class NPC;

class Object3D
{
	friend bool operator==(const Object3D& a,const Object3D& b);
	friend class Space3D;
	friend class Sphere3D;

	protected:
	TextureCollection TX;
	Vector3D pos;
	Vector3D dim;
	Vector3D dir;
	static int idMod;
	int id;
	int health;		
	//Space3D *super;
	

	double bindingRadius;
	double speed;
	double gravity;
	double default_gravity;

	public:
	vector<Space3D*> owners;
	Object3D(){};
	Object3D(Vector3D _pos,Vector3D _dim,TextureCollection _TX);
	virtual void update();
	virtual void draw();
	virtual void handleCollision(Object3D& b);
	virtual Vector3D center(){return pos+(dim*0.5);}
	virtual bool intersects(Object3D& o);
	virtual void actOn(Object3D& o){};
	virtual bool dead(){return false;}
	virtual void takeDamage(){};
	virtual void moveCenterTo(Vector3D dst){pos=dst-dim*0.5;}
	virtual bool converging(Object3D& b);
	virtual bool onSolid();
	void kill();
	void checkHealth(){if(health<=0) kill();}
    bool detectCollisions();//move to all objects
	bool forceInBounds();
	void stepForward(){pos=pos+(dir*speed);}
	void stepBack(){pos=pos-(dir*speed);}
	void forceNegative(double &f){if(f>0)	f*=-1;}
	void forcePositive(double &f){if(f<0)	f*=-1;}
	void applyGravity();
	void forceOut(Object3D& b);
	void forceOut(Player& b);
	int signOf(double d){if(d<0)return -1;else{return 1;}}
	Vector3D getDim(){return dim;}
	Vector3D getPos(){return pos;}
	Vector3D getDir(){return dir;}
	void addOwner(Space3D* s){owners.push_back(s);}
	list<Object3D*> getSignificantObjects();
};
class Stackable:public Object3D
{
	bool moveable;
	
	public:
	Stackable(){};
	Stackable(Vector3D _pos,Vector3D _dim,TextureCollection _TX):Object3D(_pos,_dim,_TX),moveable(false){};
	void update();
	void handleCollision(Object3D& b);
	bool onSolid();
};

class Space3D:public Object3D
{
	friend class Object3D;
	friend class Stackable;
	protected:
	bool draw_borders;
	list<Object3D*> objects;
	
	public:
	Space3D(){};
	Space3D(Vector3D _pos,Vector3D _dim,TextureCollection _TX,Space3D *_super, bool borders);
	void addObject(Object3D* o);
	void update();
	void draw();
	void drawBorders();
	bool containsOriginOf(Object3D& o);
	list<Object3D*>& getObjects(){return objects;}
	~Space3D();

	bool containsFully(Object3D& o){
		Vector3D tmax=center()+dim*0.5;
		Vector3D tmin=center()-dim*0.5;
		Vector3D omax=o.center()+o.getDim()*0.5;
		Vector3D omin=o.center()-o.getDim()*0.5;
		if(omax>tmax || omin<tmin)
			return true;
		return false;
	}
	
};

class Sphere3D: public Object3D
{
	protected:
	GLfloat	xrot;				
	GLfloat	yrot;	
	GLUquadricObj *quadratic;
	GLfloat r,g,b;
	int rotspeed;
	
	public:
	Sphere3D(){}
	Sphere3D(Vector3D _pos,Vector3D _dir,double radius, Space3D* containingField,TextureCollection _TX ,GLUquadricObj *_quadratic,int _rotspeed,double _speed,int _health);
	void update();	
	bool intersects(Object3D& b);
	void handleCollision(Object3D& b);
	void draw();
	Vector3D center(){return pos;}
	void actOn(Object3D& p);
	void moveCenterTo(Vector3D dst){pos=dst;}
	void takeDamage(){this->health--;checkHealth();};
	bool dead(){return health<=0;}

};

class SuperSpace3D:public Space3D
{

	protected:
	vector<Space3D> sectors;
	int n;
	double lStep;
	double hStep;
	double dStep;

	public:
	SuperSpace3D(int _n,Vector3D _dim,TextureCollection _TX,bool db);
	SuperSpace3D(){};
	~SuperSpace3D(){
	for(list<Object3D*>::iterator it=objects.begin();it!=objects.end();++it)
		delete *it;
	}
	void update();//make virtual for all objects
	void assignSectors(Object3D& c);
	int getSectorIndex(Vector3D p);
	void draw();//make virtual for all objects
	

};

class Player: public Stackable
{
	friend class Camera;
	
	Vector3D arm;
	bool move_requested;
	bool show_arm;
	
	public:
	Player(){};
	Player(Vector3D _pos,Vector3D _dim,TextureCollection _TX,int _health);
	void update();
	void draw();
	void takeDamage(){this->health--;checkHealth();};
	bool dead(){return health<=0;}
	void handleCollision(Object3D& b);
	bool converging(Object3D& b){return true;}
	
	
};

class NPC:public Player
{
	Player* player;
	
	public:
	NPC();
	NPC(Vector3D _pos,Vector3D _dim,TextureCollection _TX,Player* _player,int _health,double _speed);
	void update();
	void draw(){Object3D::draw();}
	void actOn(Object3D& p);
	bool converging(Object3D& b){return Object3D::converging(b);}


};

bool operator==(const Object3D& a,const Object3D& b);


#endif