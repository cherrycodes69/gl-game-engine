#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <vector>
#include <gl\GLAUX.H>
#include "TextureCollection.h"
#include "Vector3D.h"
#include "MilkshapeModel.h"
#include <algorithm>
#include <gl\GLU.H>
#include <list>
#include <string.h>
#include "ModelCollection.h"
using namespace::std;


class Container3D;
class Player;
class NPC;
class Object3D
{
public:
	TextureCollection TX;
	Vector3D pos;
	Vector3D dim;
	Vector3D dir;
	static int idMod;
	int id;
	Model *model;
	int health;		
	
	Container3D *super;

	double bindingRadius;
	double speed;
	double gravity;
	double default_gravity;
	Object3D(){};
	Object3D(double _x,double _y,double _z,double _l,double _h,double _d,TextureCollection _TX)
		:pos(_x,_y,_z),dim(_l,_h,_d),dir(0,0,0),bindingRadius(_l/2),gravity(0.2),speed(0.1),TX(_TX){
			dir.normalize();
			id=idMod++;
			default_gravity=gravity;
	}
	
	virtual void update();
	virtual void draw();
	bool detectCollisions();//move to all objects
	virtual void handleCollision(Object3D& b);
	virtual bool intersects(Object3D& o);
	virtual void takeDamage(){};
	void checkHealth(){if(health<0){this->kill();}}
	
	virtual Vector3D center(){return pos+(dim*0.5);}
	virtual void actOn(Object3D& o){};
	virtual bool dead(){return false;}
	virtual void moveCenterTo(Vector3D dst){pos=dst-dim*0.5;}
	void kill();

	void loadModel( const char *filename)
	{
		model=new MilkshapeModel();
		model->loadModelData(filename);

	}
	
	
    
	
	bool forceInBounds();
	void stepForward();
	void stepBack();
	virtual bool converging(Object3D& b);
	void forceNegative(double &f);
	void forcePositive(double &f);
	void applyGravity();
	virtual void forceOut(Object3D& b);
	int signOf(double d){if(d<0)return -1;else{return 1;}}
	virtual bool onSolid();
	
	
	
	
};
class Stackable:public Object3D
{
	public:
	bool moveable;
	Stackable(){};
	Stackable	(double _x,double _y,double _z,double _l,double _h,double _d,TextureCollection _TX)
				:Object3D(_x,_y,_z,_l,_h,_d,_TX)
	{
		moveable=false;
	}
	void update()
	{
		applyGravity();
		forceInBounds();
		pos.dy+=dir.dy*speed;
	}
	void handleCollision(Object3D& b)
	{
		if (moveable)
			b.forceOut(*this);
		else{
			Object3D::handleCollision(b);
			//forceOut(b);
		}
	}
	bool onSolid();
	
};
class Terrain:public Object3D
{
		public:
		double maxX,maxZ,minX,minZ;
		Terrain():Object3D(0,0,0,0,0,0,0)
		{
			maxX=0;
		};
		void findExtremes()
		{
			maxX=maxZ=-999;
			minX=minZ=999;
			for(int i=0;i<model->m_numVertices;i++)
			{
				if(model->m_pVertices[i].m_location[0] >= maxX)
					maxX=model->m_pVertices[i].m_location[0];
				if(model->m_pVertices[i].m_location[0] <= minX)
					minX=model->m_pVertices[i].m_location[0];
				if(model->m_pVertices[i].m_location[2] >= maxZ)
					maxZ=model->m_pVertices[i].m_location[2];
				if(model->m_pVertices[i].m_location[2] <= minZ)
					minZ=model->m_pVertices[i].m_location[2];

			}
			
			dim.dx=maxX-minX;
			dim.dz=maxZ-minZ;
			dim.dy=1;
		}
		void update()
		{
		};
		void draw()
		{
			//glBindTexture(GL_TEXTURE_2D,TX.textures[1]);
			glPushMatrix();
			pos.dx=-40;
			pos.dz=-40;
			glTranslatef(center().dx, center().dy, center().dz);
			model->draw();
			glPopMatrix();
			//glBindTexture(GL_TEXTURE_2D,0);
			//Object3D::draw();
		}
		void handleCollision(Object3D& b)
		{
			int i=0;
		}
		void actOn(Object3D& b)
		{
			int i=0;
		}
		bool intersects(Object3D& b)
		{
			if(maxX==0)
				findExtremes();
			if(b.pos.dx >= minX && b.pos.dx <=maxX && b.pos.dz >= minZ && b.pos.dz<=maxZ && b.pos.dy<-18.9+getHeightAt(b.center().dx,b.center().dz))
				return true;
			return false;
		}
		void forceOut(Object3D& b)
		{
			b.pos.dy=-19+getHeightAt(b.center().dx,b.center().dz);
			b.dir.dy=0;
		}
		double getHeightAt(double x,double z)
		{
			double result=0;
			//x=x+minX;
			//z=z+minZ;
			for(int i=0;i<model->m_numVertices;i++)
				if(approxCompare(0.05,model->m_pVertices[i].m_location[0],x) && approxCompare(0.05,model->m_pVertices[i].m_location[2],z))
					result=model->m_pVertices[i].m_location[1];
			if(result==0)
				for(int i=0;i<model->m_numVertices;i++)
					if(approxCompare(0.1,model->m_pVertices[i].m_location[0],x) && approxCompare(0.1,model->m_pVertices[i].m_location[2],z))
						result=model->m_pVertices[i].m_location[1];

			if(result==0)
				for(int i=0;i<model->m_numVertices;i++)
					if(approxCompare(0.2,model->m_pVertices[i].m_location[0],x) && approxCompare(0.2,model->m_pVertices[i].m_location[2],z))
						result=model->m_pVertices[i].m_location[1];
			if(result==0)
				for(int i=0;i<model->m_numVertices;i++)
					if(approxCompare(0.5,model->m_pVertices[i].m_location[0],x) && approxCompare(0.5,model->m_pVertices[i].m_location[2],z))
						result=model->m_pVertices[i].m_location[1];
			if(result==0)
				for(int i=0;i<model->m_numVertices;i++)
					if(approxCompare(1,model->m_pVertices[i].m_location[0],x) && approxCompare(1,model->m_pVertices[i].m_location[2],z))
						result=model->m_pVertices[i].m_location[1];

			
			return result;
		}
		bool approxCompare(double accuracy,double n1,double n2)
		{
			if(n1<=n2+accuracy && n1>=n2-accuracy)
				return true;
			return false;
		}
		
};

class Container3D:public Object3D
{
	public:
	list<Object3D*> objects;

	bool draw_borders;
	Container3D(){};
	Container3D(double _x,double _y,double _z,double _l,double _h,double _d,TextureCollection _TX,Container3D *_super=0, bool borders=false)
		:Object3D(_x,_y,_z,_l,_h,_d,_TX){
		TX=_TX;
		super=_super;
		draw_borders=borders;
		
	};
	void addObject(Object3D* o);
	

	void update();//make virtual for all objects
	void draw();//make virtual for all objects
	void drawBorders();
	bool containsOriginOf(Object3D& o){
		if(o.center()>center()-(dim*0.5) && o.center()<center()+(dim*0.5))
			return true;
		return false;};
	~Container3D();
		

	};

class Sphere3D: public Object3D
{

	public:
	GLfloat	xrot;				
	GLfloat	yrot;	
	GLUquadricObj *quadratic;
	GLfloat r,g,b;
	int rotspeed;

	Sphere3D(){super=0;}
	Sphere3D(double radius, Container3D* containingField,TextureCollection _TX ,GLUquadricObj *_quadratic,
			 double _dx,double _dy,double _dz,double _x,double _y,double _z,int _rotspeed,double _speed,int _health);
	
	void update();	
	bool intersects(Object3D& b);

	void handleCollision(Object3D& b);
	void draw();
	Vector3D center(){return pos;}

	void actOn(Object3D& p);
	void moveCenterTo(Vector3D dst){pos=dst;}
	void takeDamage(){
 		this->health--;
		checkHealth();
	};
	bool dead(){return health<=0;}
	string toString()
	{
		return "";
	}
};

class TopContainer:public Container3D
{

	public:
	vector<Container3D> sectors;
	int n;
	double lStep;
	double hStep;
	double dStep;
	TopContainer(int _n,double l,double h, double d,TextureCollection _TX,bool db);
	TopContainer(){};
	
	void update();//make virtual for all objects
	void findSectorFor(Object3D& c);
	void draw();//make virtual for all objects
	

};

class Player: public Stackable
{
	public:
	Vector3D arm;
	bool move_requested;
	bool show_arm;

	Player(){};
	Player(double _x,double _y,double _z,double _l,double _h,double _d,TextureCollection _TX,int _health)
		:Stackable(_x,_y,_z,_l,_h,_d,_TX){
			health=_health;
			speed=0.7;
			arm=Vector3D(0,0,0);

	move_requested=false;
	show_arm=false;
	}

	void update(){
		if(dead())
		{
			pos=Vector3D(-1000,-1000,-1000);
			
			return;
		}

		applyGravity();
		forceInBounds();
		
		bool onground=onSolid();
		if(!onground)
			int i=0;
		if(!dead() && move_requested == onground)
		{

			pos.dx+=dir.dx*speed;
			pos.dz+=dir.dz*speed;
			arm.dx+=dir.dx*speed;
			arm.dz+=dir.dz*speed;

		}
		
		if (!onground){
				pos.dy+=dir.dy*speed;
				arm.dy+=dir.dy*speed;
		}


		detectCollisions();

		move_requested=false;
		
		
	}

	void draw(){
		if(show_arm)
		{
			Object3D(arm.dx,arm.dy,arm.dz,1,1,1,30).draw();
		}
		show_arm=false;
		//Object3D::draw();
	}
	
	void takeDamage(){
 		this->health--;
		checkHealth();
	};
	bool dead(){return health<=0;}
	void handleCollision(Object3D& b);
	
	bool converging(Object3D& b){
  		return true;}

	string toString()
	{
		return "";
	}
	
};

class NPC:public Player
{
	public:
	Player* player;
	ModelCollection models;
	int rotationAngle;
	NPC();
	NPC(double _x,double _y,double _z,double _l,double _h,double _d,TextureCollection _TX,Player* _player,int _health,double _speed,ModelCollection *mod)
		:Player(_x,_y,_z,_l,_h,_d,_TX,_health)
	{
		player=_player;
		speed=_speed/2;
		models=*mod;
		rotationAngle=0;
		findExtremes();
	}
	void update()
	{
		
		if(health>0){
			dir=(player->center()-center()).normalize();
			dir.dy=-10;
			//if(!onSolid())
			//{
			//	dir.dx=0;
			//	dir.dz=0;
			//}

			applyGravity();
			forceInBounds();
			detectCollisions();
			stepForward();
		}
	}
	double getRotAngle()
	{
		Vector3D v1=Vector3D(0,0,1);
		
		v1=v1.normalize();
		Vector3D v2=(player->center()-center()).normalize();
		double cosfeta=v1.dot(v2)/(v1.getMag()*v2.getMag());
		double feta=acos(cosfeta);
		if(v2.dx<0)
			feta*=-1;
		return feta*180/3.14;
	}
	void draw()
	{

		//Object3D::draw();
		glPushMatrix();
		glTranslatef(center().dx, pos.dy, center().dz);
		
		glRotatef(getRotAngle(), 0.0f, 1.0f, 0.0f);
		MilkshapeModel &test=models.getModel();
		test.draw();
		
		glPopMatrix();
	}
	void actOn(Object3D& p);
	bool converging(Object3D& b){return Object3D::converging(b);}
	string toString()
	{
		return "";
	}
	void findExtremes()
		{	
			double maxX,maxZ,minX,minZ,maxY,minY;
			maxX=maxZ=maxY=-999;
			minX=minZ=minY=999;
			Model *model=models.models[models.at];
			
			for(int i=0;i<model->m_numVertices;i++)
			{
				if(model->m_pVertices[i].m_location[0] >= maxX)
					maxX=model->m_pVertices[i].m_location[0];
				if(model->m_pVertices[i].m_location[0] <= minX)
					minX=model->m_pVertices[i].m_location[0];
				if(model->m_pVertices[i].m_location[1] >= maxY)
					maxY=model->m_pVertices[i].m_location[1];
				if(model->m_pVertices[i].m_location[1] <= minY)
					minY=model->m_pVertices[i].m_location[1];
				if(model->m_pVertices[i].m_location[2] >= maxZ)
					maxZ=model->m_pVertices[i].m_location[2];
				if(model->m_pVertices[i].m_location[2] <= minZ)
					minZ=model->m_pVertices[i].m_location[2];

			}
			dim.dx=abs(maxX-minX);
			dim.dz=abs(maxZ-minZ);
			dim.dy=abs(maxY-minY);
		}
};

bool operator==(const Object3D& a,const Object3D& b);


#endif