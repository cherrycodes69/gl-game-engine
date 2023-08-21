#ifndef CAMERA_H
#define CAMERA_H
#include "Object3D.h"


class Camera
{
	public:
	Vector3D pos,dir,up;
	double speed;
	
	Camera(){}
	Camera(Vector3D _pos,Vector3D _dir,Vector3D _up,double _speed):pos(_pos),dir(_dir),up(_up),speed(_speed){}
	void go(int direction)
	{
		double s=speed;
		if(direction<0)
			s*=-1;

		Vector3D v=dir.subtract(pos);
	
		pos=pos+(v*s);
		dir=dir+(v*s);
		//up=up+(v*s);
		
	}

	void rotate(double s)
	{
		//double s=speed;
		//if(direction<0)
		//	s*=-1;

		Vector3D v=dir.subtract(pos);	

		dir.dz = (float)(pos.dz + sin(s)*v.dx + cos(s)*v.dz);
		dir.dx = (float)(pos.dx + cos(s)*v.dx - sin(s)*v.dz);
	}
	void refresh()
	{
		glLoadIdentity();
		gluLookAt(pos.dx,  pos.dy , pos.dz,	
			  dir.dx, dir.dy, dir.dz,	
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

	
		angle_y = (float)( (mid_x - mousePos.x) ) / 1000;		
		angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

	
		dir.dy += angle_z * 10;

	
		rotate(-angle_y); 
	}
};


#endif