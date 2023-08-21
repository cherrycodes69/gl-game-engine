#include "Object3D.h"
Player::Player(Vector3D _pos,Vector3D _dim,TextureCollection _TX,int _health)
		:Stackable(_pos,_dim,_TX){
		health=_health;
		speed=0.1;
		arm=Vector3D(0,0,0);
		move_requested=false;
		show_arm=false;
}
void Player::update(){
	if(dead())
	{
		pos=Vector3D(-1000,-1000,-1000);
			
		return;
	}
	bool onground=onSolid();
	//if(!onground)
		applyGravity();
	forceInBounds();
		
	
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
void Player::handleCollision(Object3D& b)
{
	Object3D::handleCollision(b);
	b.actOn(*this);
}



void Player::draw(){
	if(show_arm)
	{
		Object3D(arm,Vector3D(1,1,1),30).draw();
	}
	show_arm=false;
}