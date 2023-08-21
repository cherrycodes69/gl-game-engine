#include "Object3D.h"


//**************************OBJECT3D**************************
void Object3D::forceInBounds()
{
	Container3D *bounds;
	bounds=super;
	if(super->super!=0)
		bounds=super->super;
	
	if( pos.dx>bounds->pos.dx+bounds->dim.dx)//if reach x bounds, reverse{
		forceNegative(dir.dx);
	else if( pos.dx<bounds->pos.dx)
		forcePositive(dir.dx);

	if(pos.dy<bounds->pos.dy)
		forcePositive(dir.dy);
	else if ( pos.dy>bounds->pos.dy+bounds->dim.dy)
		forceNegative(dir.dy);
	
	if(pos.dz>bounds->pos.dz+bounds->dim.dz)
		forceNegative(dir.dz);
	else if ( pos.dz<bounds->pos.dz)
		forcePositive(dir.dz);

}
void Object3D::stepForward(){pos=pos+(dir*speed);}
void Object3D::stepBack(){pos=pos-(dir*speed);}
void Object3D::forceNegative(double &f){if(f>0)	f*=-1;}
void Object3D::forcePositive(double &f){if(f<0)	f*=-1;}

void Object3D::draw()
{
		 
		double dl=dim.dx/2;
		double dh=dim.dy/2;
		double dd=dim.dz/2;

		glPushMatrix();
		glColor4f(0.0,0.0,0.5,0.5);
		glTranslatef(pos.dx+dl, pos.dy+dh, pos.dz+dd);
		
		glNormal3f(0,0,-1);
		glBegin(GL_QUADS); 
		glVertex3f( dl, dh, dd); //up right corner
		glVertex3f( -dl, dh, dd); //up left corner
		glVertex3f(-dl,-dh, dd); //down left
		glVertex3f( dl,-dh, dd);//down right
		glEnd();
		glNormal3f(0,0,1);
		glBegin(GL_QUADS); 
		glVertex3f( dl, dh, -dd); //up right corner
		glVertex3f( -dl, dh, -dd); //up left corner
		glVertex3f(-dl,-dh, -dd); //down left
		glVertex3f( dl,-dh, -dd);//down right
		glEnd();
		glNormal3f(0,-1,0);
		glBegin(GL_QUADS); 
		glVertex3f( dl, dh, dd); //up right corner
		glVertex3f( -dl, dh, dd); //up left corner
		glVertex3f(-dl,dh, -dd); //down left
		glVertex3f( dl,dh, -dd);//down right
		glEnd();
		glNormal3f(0,1,0);
		glBegin(GL_QUADS); 
		glVertex3f( dl, -dh, dd); //up right corner
		glVertex3f( -dl, -dh, dd); //up left corner
		glVertex3f(-dl,-dh, -dd); //down left
		glVertex3f( dl,-dh, -dd);//down right
		glEnd();
		glNormal3f(-1,0,0);
		glBegin(GL_QUADS); 
		glVertex3f( dl, -dh, -dd); //up right corner
		glVertex3f( dl, -dh, dd); //up left corner
		glVertex3f(dl,dh, dd); //down left
		glVertex3f( dl,dh, -dd);//down right
		glEnd();
		glNormal3f(1,0,0);
		glBegin(GL_QUADS); 
		glVertex3f( -dl, -dh, -dd); //up right corner
		glVertex3f( -dl, -dh, dd); //up left corner
		glVertex3f(-dl,dh, dd); //down left
		glVertex3f( -dl,dh, -dd);//down right
		glEnd();
		glColor4f(1.0,1.0,1.0,1.0); 
		glPopMatrix();
}
bool Object3D::converging(Object3D& b)
{
	double distanceBefore = sqrt(pow((center().dx-b.center().dx),2)+pow((center().dy-b.center().dy),2)+pow((center().dz-b.center().dz),2));
	double distanceAfter = sqrt(pow(((center().dx+dir.dx*speed)-(b.center().dx+b.dir.dx*b.speed)),2)+pow(((center().dy+dir.dy*speed)-(b.center().dy+b.dir.dy*b.speed)),2)+pow(((center().dz+dir.dz*speed)-(b.center().dz+b.dir.dz*b.speed)),2));

	if(distanceAfter-distanceBefore<=0)
		return true;
	return false;
}
void Object3D::handleCollisions()
{
	if(super==NULL)
		return;

	for(std::list<Object3D*>::iterator it=super->objects.begin();it!=super->objects.end();++it)
	{
		if( !(*this==**it) && (*it)->intersects(*this))
		{
			(*it)->calculateNewDirection(*this);
		}
	}	
}

void Object3D::update()
{
	
//	forceSolid();
	applyGravity();
//	forceInBounds();
//	handleCollisions();
	stepForward();

	
	
};
void Sphere3D::update()
{
	

	handleCollisions();
	forceInBounds();
	stepForward();
	
};
void Object3D::calculateNewDirection(Object3D& b)
{
	


	//if(converging(b))
	//if(intersects(b) && converging(b))
	//	b.stepBack();
		
	Vector3D dif=b.center()-center();

		if(abs(dif.dx)>abs(dif.dy) && abs(dif.dx)>abs(dif.dz)){ //x axis is the normal
			b.dir.dx=b.dir.dx*-1;
			b.pos.dx+=(dif.dx/dif.dx)*(b.bindingRadius+bindingRadius)-abs(dif.dx);
		}
		else if(abs(dif.dy)>abs(dif.dx) && abs(dif.dy)>abs(dif.dz)){ //x axis is the normal
			b.dir.dy=b.dir.dy*-1;
			b.pos.dy+=(dif.dx/dif.dx)*(b.bindingRadius+bindingRadius)-abs(dif.dy);
		}
		else if(abs(dif.dz)>=abs(dif.dy) && abs(dif.dz)>=abs(dif.dx)){ //x axis is the normal
			b.dir.dz=b.dir.dz*-1;
			b.pos.dz+=(dif.dx/dif.dx)*(b.bindingRadius+bindingRadius)-abs(dif.dz);
		}
	//}
		
}
bool Object3D::intersects(Object3D& o){
	Vector3D c(center()),oc(o.center());
	double maxX(c.dx+(dim.dx*0.5)),omaxX((oc.dx+(o.dim.dx*0.5)));
	double maxY(c.dy+(dim.dy*0.5)),omaxY((oc.dy+(o.dim.dy*0.5)));
	double maxZ(c.dz+(dim.dz*0.5)),omaxZ((oc.dz+(o.dim.dz*0.5)));

	double minX(c.dx-(dim.dx*0.5)),ominX((oc.dx-(o.dim.dx*0.5)));
	double minY(c.dy-(dim.dy*0.5)),ominY((oc.dy-(o.dim.dy*0.5)));
	double minZ(c.dz-(dim.dz*0.5)),ominZ((oc.dz-(o.dim.dz*0.5)));
		
	if((omaxX>=minX && ominX<=maxX)&& (omaxY>=minY && ominY<=maxY) &&(omaxZ>=minZ && ominZ<=maxZ))
   		return true;
	return false;
}

bool operator==(const Object3D& a,const Object3D& b)
{
	if(a.pos==b.pos && a.dir==b.dir)
		return true;
	return false;
}

void Object3D::applyGravity()
	{
		Container3D *bounds;
		bounds=super;
		if(super->super!=0)
			bounds=super->super;
		if((center()-(dim*0.5)).dy < bounds->pos.dy){
			pos.dy=bounds->pos.dy;
			gravity=0;
			dir.dy=0;
			return;
		}
			
		dir.dy=dir.dy-gravity;
		gravity*=1.1;
		dir.normalize();

	}
//**************************SPHERE3D**************************
Sphere3D::Sphere3D(double _bindingRadius,Container3D* containingField,TextureCollection _TX,GLUquadricObj *_quadratic,
				   double _dx,double _dy,double _dz,double _x,double _y,double _z,int _rotspeed,double _speed)
				   :TX(_TX),Object3D(_x,_y,_z,_bindingRadius*2,_bindingRadius*2,_bindingRadius*2),rotspeed(_rotspeed)
{
	bindingRadius=_bindingRadius;
	speed=_speed;
	quadratic=_quadratic;
	
	super=containingField;


	dir.dx=_dx;
	dir.dy=_dy;
	dir.dz=_dz;
	dir.normalize();

	
}



bool Sphere3D::intersects(Object3D& b)
{
	double distance = sqrt(pow((pos.dx-b.pos.dx),2)+pow((pos.dy-b.pos.dy),2)+pow((pos.dz-b.pos.dz),2));

	if(distance < bindingRadius + b.bindingRadius )
		return true;
	return false;
}

void Sphere3D::calculateNewDirection(Object3D& b)
{

		Vector3D v=Vector3D(b.center()-center());
		v=v.normalize()*(bindingRadius+b.bindingRadius-v.getMag());
		pos=pos-v;

		Vector3D xA = Vector3D(b.pos-pos);
		xA=xA.normalize();

		Vector3D U1x= xA*(xA.dot(dir));
		Vector3D U1y= dir-(U1x);
		Vector3D U2x =xA.invert()*(xA.invert().dot(b.dir));
		Vector3D U2y =b.dir-(U2x);

		Vector3D V1x=((U1x)+((U2x))-(U1x-(U2x)))*(0.5);
		Vector3D V2x=((U1x)+((U2x))-(U2x-(U1x)))*(0.5);
		
		
		dir=Vector3D(V1x+U1y).normalize();
		b.dir=Vector3D(V2x+U2y).normalize();



}


void Sphere3D::draw()
{

	glPushMatrix();
	glTranslatef(pos.dx,pos.dy,pos.dz);
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	xrot+=rotspeed;yrot+=rotspeed;


	gluQuadricDrawStyle  (quadratic, GLU_FILL);
	gluQuadricNormals    (quadratic, GLU_FLAT);
	gluQuadricOrientation(quadratic, GLU_OUTSIDE);


	gluSphere(quadratic,bindingRadius,32,32);
	glPopMatrix();


}

//**************************CONTAINER3D**************************
void Container3D::addObject(Object3D* o)
{
	o->super=this;
	objects.push_back(o);

}

void Container3D::update()
{
	for(list<Object3D*>::iterator it=objects.begin();it!=objects.end();++it)
			(*it)->update();
}


void Container3D::draw()
{
	for(list<Object3D*>::iterator it=objects.begin();it!=objects.end();++it)
	{
		(*it)->draw();
	}
	if(draw_borders)
	{
		
		Object3D::draw();
	}
}
Container3D::~Container3D()
{
		for(list<Object3D*>::iterator it=objects.begin();it!=objects.end();++it)
				delete (*it);

}

//**************************TOPCONTAINER#D**************************

TopContainer::TopContainer(int _n,double l,double h, double d,bool db=false)
	{
		//glLoadIdentity();
		n=_n;
		  dim.dx=l;
		dim.dy=h;
		 dim.dz=d;
		 draw_borders=!db;
		lStep=l/n;
		hStep=h/n;
		dStep=d/n;
		pos.dx=0-l/2;
		pos.dy=0-h/2;
		pos.dz=0-d/2;
		int i=0,j=0,k=0;
		for( i=0;i<n;++i)
			for( j=0;j<n;++j)
				for( k=0;k<n;++k)
					sectors.push_back(Container3D(pos.dx+i*lStep,pos.dy+j*hStep,pos.dz+k*dStep,lStep,hStep,dStep,this,db));
	
		//sectors.push_back(Container3D(originX+hStep*i-hStep/8,originY+vStep*j-vStep/8,hStep*1.25,vStep*1.25));
		dim.dx=i*lStep;
		dim.dy=j*hStep;
		dim.dz=k*dStep;

		
	};
void TopContainer::update()
{
	for(list<Object3D*>::iterator it=objects.begin();it!=objects.end();it++) //for all objects in field, distribute to sectors
	{
		Container3D& newSector=findSectorFor(**it);
		newSector.addObject(*it);
		it=objects.erase(it);
		if(it==objects.end())
			break;
	}

	for(int i=0;i<sectors.size();++i)
	{
		sectors[i].update();
		for(list<Object3D*>::iterator it=sectors[i].objects.begin();it!=sectors[i].objects.end();++it)
		{
			bool cont=sectors[i].containsOriginOf(**it);
				if(!cont)
				{

 					Container3D& newSector=findSectorFor(**it);
					newSector.addObject(*it);
					it=sectors[i].objects.erase(it);
					if(it==sectors[i].objects.end())
						break;
				}

			
			}	
		}
}
void TopContainer::draw()
{
	if(draw_borders)
		Object3D::draw();
	for(vector<Container3D>::iterator it=sectors.begin();it!=sectors.end();++it)
	{
		it->draw();
	}
}
Container3D& TopContainer::findSectorFor(Object3D& c)
{
	if(c.dir.dx!=c.dir.dx ){//|| c.dir.dy!=c.dir.dy || c.dir.dz!=c.dir.dz || c.pos.dx!=c.dir.dx || c.pos.dy!=c.dir.dy || c.pos.dz!=c.dir.dz){ //#ind
        c.dir=Vector3D(0,0,0);
		c.pos=Vector3D(0,0,0);
		return *this;
	}

	vector<Container3D>::iterator it;
	int xI=abs(c.pos.dx+c.dim.dx-pos.dx)/lStep;
	int yI=abs(c.pos.dy+c.dim.dy-pos.dy)/hStep;
	int zI=abs(c.pos.dz+c.dim.dz-pos.dz)/dStep;
	
	
	if(xI>=n) xI=n-1;
	if( yI>=n ) yI=n-1;
	if( zI>=n) zI=n-1;
	//	return *this;
	int index=xI*n*n+yI*n+zI;
	
	return sectors[index];

}