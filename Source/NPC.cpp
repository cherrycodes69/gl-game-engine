#include "Object3D.h"

NPC::NPC(Vector3D _pos,Vector3D _dim,TextureCollection _TX,Player* _player,int _health,double _speed)
		:Player(_pos,_dim,_TX,_health)
{
	player=_player;
	speed=_speed;
		
}
void NPC::update()
{
	if(health>0){
		dir=(player->center()-center()).normalize();
		if(!onSolid())
		{
			dir.dx=0;
			dir.dz=0;
		}
		applyGravity();
		forceInBounds();
		detectCollisions();
		stepForward();
	}
}
void NPC::actOn(Object3D& p)
{
	p.takeDamage();
}