#ifndef CAMERA_H
#define CAMERA_H
#include "Object3D.h"


class Camera
{
	public:
	Vector3D pos,arm,up;
	double speed;
	Player* body;
	SuperSpace3D* aware;
	double lengthOfArm;
	Camera(){}
	Camera(Vector3D _pos,Vector3D _dir,Vector3D _up,double _speed,SuperSpace3D* _aware,int _health)
		:aware(_aware),pos(_pos),arm(_dir),up(_up),speed(_speed)
	{
		body=new Player(Vector3D(pos.dx,pos.dy-0.5,pos.dz),Vector3D(1,1,1),TextureCollection(),_health);
		aware->addObject(body);
		speed=body->speed;
		lengthOfArm=5;
	}
	Vector3D center(){return pos+(body->dim*0.5);}

	
	void go(int direction)
	{
		if(!body->onSolid())
			return;
		Vector3D newdir((arm-pos).normalize());
		body->dir.dx=newdir.dx*direction;
		body->dir.dz=newdir.dz*direction;

		body->move_requested=true;
	}

	void drawArm()
	{
		body->show_arm=true;
	}
	void strafe(int direction)
	{
		if(!body->onSolid())
			return;
		Vector3D tmp=arm.cross(pos).normalize();
		body->dir.dx=tmp.dx*direction;
		body->dir.dz=tmp.dz*direction;

		body->move_requested=true;
	}
	void jump()
	{
		if(body->move_requested==false){
			body->dir.dx=0;
			body->dir.dz=0;
		}
		body->pos.dy+=0.1;
		body->dir.dy+=3;
	}
	void rotateH(double s)
	{	
		Vector3D v=arm.subtract(pos);	
		arm.dz = (float)(pos.dz + sin(s)*v.dx + cos(s)*v.dz);
		arm.dx = (float)(pos.dx + cos(s)*v.dx - sin(s)*v.dz);
	}
	
	void refresh()
	{
		pos=body->pos;
		
		body->arm=arm;

		glLoadIdentity();
		Vector3D cpos=center();
		gluLookAt(cpos.dx,  cpos.dy , cpos.dz,	
			  arm.dx, arm.dy, arm.dz,	
			  up.dx,   up.dy,   up.dz);
	}


	void goToMouse(int wndWidth, int wndHeight)
	{
		POINT mousePos;	
		int mid_x = wndWidth  >> 1;	
		int mid_y = wndHeight >> 1;	
		float angle_y  = 0;				
		float angle_z  = 0;							
	
		GetCursorPos(&mousePos);	
		SetCursorPos(mid_x, mid_y);	

		angle_z = (float)( (mid_x - mousePos.x) ) / 1000;		
		angle_y = (float)( (mid_y - mousePos.y) ) / 1000;

		arm.dy += angle_y *5;
		rotateH(-angle_z); 

		Vector3D dif(body->pos-pos);
		arm=arm+dif;
		arm=pos+(arm-pos).normalize()*lengthOfArm;
	}
	
};


#endif