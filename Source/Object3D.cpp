#include "Object3D.h"
int Object3D::idMod=0;

Object3D::Object3D(Vector3D _pos,Vector3D _dim,TextureCollection _TX)
		:pos(_pos),dim(_dim),dir(0,0,0),bindingRadius(_dim.dx/2),gravity(0.2),speed(0.1),TX(_TX){
			dir.normalize();
			id=idMod++;
			default_gravity=gravity;
}
list<Object3D*> Object3D::getSignificantObjects()
{
	list<Object3D*> result=list<Object3D*>();

	for(int i=1;i<owners.size();++i)
		result.insert(result.end(),owners[i]->objects.begin(),owners[i]->objects.end());
	result.unique();
	if(result.size()>10)
		int z=0;
	return result;

}
void Object3D::update()
{
	applyGravity();
	stepForward();
};


bool Object3D::onSolid()
{
	bool result=false;

	Space3D* bounds=owners[0];
	//if(bounds!=0)
	//	bounds=bounds->owners[0];

	if((center()-(dim*0.5)).dy <= bounds->pos.dy ){
		pos.dy=bounds->pos.dy;
		result=true;
	}
	return result;
}

void Object3D::handleCollision(Object3D& b)
{

	Vector3D dif=b.center()-center();

		if(abs(dif.dx)>abs(dif.dy) && abs(dif.dx)>abs(dif.dz)) //x axis is the normal
			b.dir.dx=b.dir.dx*-1;
		else if(abs(dif.dy)>abs(dif.dx) && abs(dif.dy)>abs(dif.dz)) //x axis is the normal
			b.dir.dy=b.dir.dy*-1;
		else if(abs(dif.dz)>abs(dif.dy) && abs(dif.dz)>abs(dif.dx)) //x axis is the normal
			b.dir.dz=b.dir.dz*-1;
		
			forceOut(b);
		
}

void Object3D::forceOut(Object3D& b)
{
	Vector3D dif=b.center()-center();

	if(abs(dif.dx)>abs(dif.dy) && abs(dif.dx)>abs(dif.dz))
		b.pos.dx+=signOf(dif.dx)*((b.bindingRadius+bindingRadius)-abs(dif.dx));
	else if(abs(dif.dz)>=abs(dif.dy) && abs(dif.dz)>=abs(dif.dx))
		b.pos.dz+=signOf(dif.dz)*((b.bindingRadius+bindingRadius)-abs(dif.dz));
	else if(abs(dif.dy)>=abs(dif.dx) && abs(dif.dy)>=abs(dif.dz))
		b.pos.dy+=signOf(dif.dy)*((b.bindingRadius+bindingRadius)-abs(dif.dy));
		
	b.actOn(*this);
}

bool Object3D::forceInBounds()
{
	bool result=false;
	Space3D* bounds=owners[0];
	//if(!bounds->owners.empty())
	//	bounds=bounds->owners[0];
 	
	if( pos.dx>bounds->pos.dx+bounds->dim.dx){//if reach x bounds, reverse{
		//forceNegative(dir.dx);
		pos.dx=bounds->pos.dx+bounds->dim.dx-dim.dx/2;
		result=true;
	}
	else if( pos.dx<bounds->pos.dx){
		//forcePositive(dir.dx);
		pos.dx=bounds->pos.dx+dim.dx/2;
		result=true;
	}
	if(pos.dy<bounds->pos.dy){
		//forcePositive(dir.dy);
		pos.dy=bounds->pos.dy;
		result=true;
	}
	else if ( pos.dy>bounds->pos.dy+bounds->dim.dy){
		//forceNegative(dir.dy);
		pos.dy=bounds->pos.dy+bounds->dim.dy-dim.dy/2;
		result=true;
	}
	if(pos.dz>bounds->pos.dz+bounds->dim.dz){
		//forceNegative(dir.dz);
		pos.dz=bounds->pos.dz+bounds->dim.dz-dim.dx/2;
		result=true;
	}
	else if ( pos.dz<bounds->pos.dz){
		//forcePositive(dir.dz);
		pos.dz=bounds->pos.dz+dim.dz/2;
		result=true;
	}
	if(result){
		dir=Vector3D(0,0,0);
		gravity=0;
	}
	return result;

}

void Object3D::applyGravity()
{
	bool firmGround=onSolid();
	if(firmGround)
	{
		dir.dy=0;
		gravity=0;
		return;
	}
	if(dir.dy>=0 && !firmGround)
		gravity=default_gravity;	
	dir.dy-=gravity;
	gravity*=1.01;

}

void Object3D::draw()
{

		double dl=dim.dx/2;
		double dh=dim.dy/2;
		double dd=dim.dz/2;
		
		glBindTexture(GL_TEXTURE_2D,TX.getTX());
		glPushMatrix();
		glTranslatef(center().dx, center().dy, center().dz);
		glNormal3f(0,0,1);
		glBegin(GL_QUADS); 
		glTexCoord2f(1.0f, 1.0f);glVertex3f( dl, dh, dd); //up right corner
		glTexCoord2f(0.0f, 1.0f);glVertex3f( -dl, dh, dd); //up left corner
		glTexCoord2f(0.0f, 0.0f);glVertex3f(-dl,-dh, dd); //down left
		glTexCoord2f(1.0f, 0.0f);glVertex3f( dl,-dh, dd);//down right
		glEnd();
		glNormal3f(0,0,-1);
		glBegin(GL_QUADS); 
		glTexCoord2f(1.0f, 1.0f);glVertex3f( dl, dh, -dd); //up right corner
		glTexCoord2f(0.0f, 1.0f);glVertex3f( -dl, dh, -dd); //up left corner
		glTexCoord2f(0.0f, 0.0f);glVertex3f(-dl,-dh, -dd); //down left
		glTexCoord2f(1.0f, 0.0f);glVertex3f( dl,-dh, -dd);//down right
		glEnd();
		glNormal3f(0,1,0);
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f, 0.0f);glVertex3f( dl, dh, dd); //up right corner
		glTexCoord2f(1.0f, 0.0f);glVertex3f( -dl, dh, dd); //up left corner
		glTexCoord2f(1.0f, 1.0f);glVertex3f(-dl,dh, -dd); //down left
		glTexCoord2f(0.0f, 1.0f);glVertex3f( dl,dh, -dd);//down right
		glEnd();
		glNormal3f(0,-1,0);
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f, 0.0f);glVertex3f( dl, -dh, dd); //up right corner
		glTexCoord2f(1.0f, 0.0f);glVertex3f( -dl, -dh, dd); //up left corner
		glTexCoord2f(1.0f, 1.0f);glVertex3f(-dl,-dh, -dd); //down left
		glTexCoord2f(0.0f, 1.0f);glVertex3f( dl,-dh, -dd);//down right
		glEnd();
		glNormal3f(1,0,0);
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f, 0.0f);glVertex3f( dl, -dh, -dd); //up right corner
		glTexCoord2f(1.0f, 0.0f);glVertex3f( dl, -dh, dd); //up left corner
		glTexCoord2f(1.0f, 1.0f);glVertex3f(dl,dh, dd); //down left
		glTexCoord2f(0.0f, 1.0f);glVertex3f( dl,dh, -dd);//down right
		glEnd();
		glNormal3f(-1,0,0);
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f, 0.0f);glVertex3f( -dl, -dh, -dd); //up right corner
		glTexCoord2f(1.0f, 0.0f);glVertex3f( -dl, -dh, dd); //up left corner
		glTexCoord2f(1.0f, 1.0f);glVertex3f(-dl,dh, dd); //down left
		glTexCoord2f(0.0f, 1.0f);glVertex3f( -dl,dh, -dd);//down right
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D,0);
		
}

bool Object3D::converging(Object3D& b)
{
	double distanceBefore = sqrt(pow((center().dx-b.center().dx),2)+pow((center().dy-b.center().dy),2)+pow((center().dz-b.center().dz),2));
	double distanceAfter = sqrt(pow(((center().dx+dir.dx*speed)-(b.center().dx+b.dir.dx*b.speed)),2)+pow(((center().dy+dir.dy*speed)-(b.center().dy+b.dir.dy*b.speed)),2)+pow(((center().dz+dir.dz*speed)-(b.center().dz+b.dir.dz*b.speed)),2));

	if(distanceAfter-distanceBefore<=0)
		return true;
	return false;
}

bool Object3D::detectCollisions()
{
	bool result=false;
	list<Object3D*> sigOb=getSignificantObjects();
	//if(sigOb.size()==0)
	//	return 0;

	for(std::list<Object3D*>::iterator it=sigOb.begin();it!=sigOb.end();++it)
	{
		if( !(*this==**it) && (*it)->intersects(*this))
		{
			(*it)->handleCollision(*this);
			result=true;
		}
	}	
	return result;
}

void Object3D::kill()
{
		owners.erase(owners.begin()+1,owners.end());
		pos=Vector3D(0,0,-100);
		dir=Vector3D();
}

bool Object3D::intersects(Object3D& o){
	Vector3D c(center()),oc(o.center());
	Vector3D cmax(center()+(dim*0.5)),omax(o.center()+(o.dim*0.5));
	Vector3D cmin(center()-(dim*0.5)),omin(o.center()-(o.dim*0.5));
	
	if((omax.dx>=cmin.dx && omin.dx<=cmax.dx)&& (omax.dy>=cmin.dy && omin.dy<=cmax.dy) &&(omax.dz>=cmin.dz && omin.dz<=cmax.dz))
   		return true;
	return false;
}

bool operator==(const Object3D& a,const Object3D& b)
{
	if(a.id==b.id)
		return true;
	return false;
}
