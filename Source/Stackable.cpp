#include "Object3D.h"
bool Stackable::onSolid()
{
	bool result=false;
	list<Object3D*> sigOb=getSignificantObjects();
	/*if(sigOb.size()==0)
		return 0;
	*/
	if(sigOb.size()>=10)
		int Z=0;
	for(std::list<Object3D*>::iterator it=sigOb.begin();it!=sigOb.end();++it)
	{
		if( !(*this==**it) && (*it)->intersects(*this))
		{
			(*it)->forceOut(*this);	
			//forceOut(**it);
			
			if(center().dy-dim.dy*0.5>=(*it)->center().dy+(*it)->getDim().dy*0.5)
				result=true;
		}
	}
	
	if(Object3D::onSolid())
		result=true;

	return result;
}
void Stackable::update()
{
	if(dir.dy==0 && gravity==0)
		return;
	applyGravity();
	forceInBounds();
	pos.dy+=dir.dy*speed;
}
void Stackable::handleCollision(Object3D& b)
{
	if (moveable)
		b.forceOut(*this);
	else{
		Object3D::handleCollision(b);
		//forceOut(b);
	}
}