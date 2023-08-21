#include "Object2D.h"

/*void Circle2D::updateEdges()
{

	for(std::vector<Edge>::iterator it=edges.begin();it!=edges.end();++it)
	{

		it->update();
	}

}*/

double Vector2D::getMag ()
{
	return sqrt(pow(dx,2)+pow(dy,2));
}

double Vector2D::dot(const Vector2D &other)
{
	return this->dx*other.dx+this->dy*other.dy;
}

Vector2D Vector2D::multiply(double f)
{
	Vector2D r(*this);
	r.dx*=f;
	r.dy*=f;
	return r;
}



Vector2D Vector2D::invert()
{
	Vector2D r(*this);
	r.dx*=-1;
	r.dy*=-1;
	return r;
}

Vector2D Vector2D::normalize()
{
	Vector2D r(*this);
	double len=getMag();
	r.dy/=len;
	r.dx/=len;
	return r;
}



Circle2D::Circle2D(double _bindingRadius,Container2D* containingField,TextureCollection _TX,double _dx,double _dy,double _x,double _y):TX(_TX),Object2D(_x,_y,_bindingRadius*2,_bindingRadius*2)
{
	bindingRadius=_bindingRadius;
	speed=0.2;


	super=containingField;
	x=_x;
	y=_y;

	dir.dx=_dx;
	dir.dy=_dy;

	dir.normalize();
	
}

void Circle2D::update()
{

		calculateMovements();
		
}

bool Circle2D::intersects(const Object2D& b)
{
	double distance = sqrt(pow((x-b.x),2)+pow((y-b.y),2));

	if(distance < bindingRadius + b.bindingRadius )
		return true;
	return false;
}
void Circle2D::calculateNewDirection(Circle2D& b)
{

		Vector2D direction=Vector2D(b.x-x,b.y-y);
		direction=direction.normalize();
		// Projection of the velocities in these axes
		double va1=(dir.dx*direction.dx+dir.dy*direction.dy), vb1=(-dir.dx*direction.dy+dir.dy*direction.dx);
		double va2=(b.dir.dx*direction.dx+b.dir.dy*direction.dy), vb2=(-b.dir.dx*direction.dy+b.dir.dy*direction.dx);
		// New velocities in these axes (after collision): ed<=1,  for elastic collision ed=1
		int ed=1;
		double vaP1=va1 + (1+ed)*(va2-va1)/(1+bindingRadius/b.bindingRadius);
		//double vaP2=va2 + (1+ed)*(va1-va2)/(1+b.bindingRadius/bindingRadius);
		// Undo the projections
		dir=Vector2D(vaP1*direction.dx-vb1*direction.dy,vaP1*direction.dy+vb1*direction.dx);
		dir.normalize();
		//b.dir=Vector2D(vaP2*direction.dx-vb2*direction.dy,vaP2*direction.dy+vb2*direction.dx);
		//b.dir.normalize();
		//   TX.nextTX();
		//   b.TX.nextTX();
		
	if(converging(b))
	{
		dir.subtract(direction);
		dir.normalize();
		b.dir.add(direction);
		b.dir.normalize();
	}
	
//
}
void Circle2D::handleCollisions()
{


	//Object2D closest=edges.back().other;
	for(std::vector<Circle2D>::iterator it=super->objects.begin();it<super->objects.end();++it)
	{
		if(converging(*it)  && !(*this==*it) && intersects(*it))
		{
			stepBack();
			/*
			Vector2D v=Vector2D(x-it->x,y-it->y);
			Vector2D t(v);
			t.normalize();
			t.multiply(bindingRadius+it->bindingRadius-v.getMag());
			x+=t.dx;
			y+=t.dy;*/
			//it->stepBack();

			calculateNewDirection(*it);
			//it->stepForward();
			
		
		}
	}

	


	
}
void Circle2D::calculateMovements()
{
	
	handleCollisions();
	forceInBounds();
	stepForward();
	
};
void Circle2D::forceInBounds()
{
	if(x>10 )//if reach x bounds, reverse
	{
		forceNegative(dir.dx);
	}
	else if( x<-10)
	{
		forcePositive(dir.dx);
	}
	if(y<-10)
	{
		forcePositive(dir.dy);
	}
	else if ( y>10 )
	{
		forceNegative(dir.dy);
	}


}
void Circle2D::stepForward()
{
	y+=dir.dy*speed;
	x+=dir.dx*speed;

}
void Circle2D::stepBack()
{
	y-=dir.dy*speed;
	x-=dir.dx*speed;

}

void Circle2D::forceNegative(double &f)
{
	if(f>0)
		f*=-1;
}
void Circle2D::forcePositive(double &f)
{
	if(f<0)
		f*=-1;
}
void Circle2D::draw()
{

	
	glLoadIdentity();
	glTranslatef(x,y,-40.0f);

	
	float fittingQuad=l/2;
	fittingQuad*=0.8; 

	glColor3f(1.0,1.0,1.0); 
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, TX.getTX());
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	gluSphere(quadratic,1.3f,32,32);
	
	glBegin(GL_QUADS); 
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( fittingQuad, fittingQuad, 0.0f); //up right corner

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f( -1*fittingQuad, fittingQuad, 0.0f); //up left corner

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1*fittingQuad,-1*fittingQuad, 0.0f); //down left

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( fittingQuad,-1*fittingQuad, 0.0f);//down right

	glEnd();
	

}
bool Circle2D::converging(const Object2D& b)
{
	double distanceBefore = sqrt(pow((x-b.x),2)+pow((y-b.y),2));
	double distanceAfter = sqrt(pow(((x+dir.dx*speed)-(b.x+b.dir.dx*b.speed)),2)+pow(((y+dir.dy*speed)-(b.y+b.dir.dy*b.speed)),2));

	if(distanceAfter-distanceBefore<=0)
		return true;
	return false;
}

bool operator==(const Vector2D& a,const Vector2D& b)
{
	if(a.dx==b.dx && a.dy==b.dy)
		return true;
	return false;
}
bool operator==(const Object2D& a,const Object2D& b)
{
	if(a.x == b.x && a.y == b.y && a.dir==b.dir)
		return true;
	return false;
}
void Container2D::addObject(Circle2D o)
{
	o.super=this;
	objects.push_back(o);

}

void Container2D::update()
{
	for(unsigned int i=0;i<objects.size();i++)
		//if(contains(objects[i]))
			objects[i].update();
}
void Container2D::draw()
{
	for(vector<Circle2D>::iterator it=objects.begin();it!=objects.end();++it)
	{
		it->draw();
	}
}
void TopContainer::update()
{
	int i;
	for(vector<Circle2D>::iterator it=objects.begin();it<objects.end()-1;++it) //for all objects in field, distribute to sectors
	{
		//it->draw();
		it->forceInBounds();
		it->stepForward();
		
		Container2D& newSector=findSectorFor(*it);
		if(&newSector!=this)
		{

			it->super=&newSector;
			//it->stepBack();

			newSector.addObject(*it);
			if(objects.empty())// && it==objects.end()-1)
			{
	//		objects.erase(it);
				break;
			}else{
				it=objects.erase(it);
			}
			//it->draw();
		}
			
	}
			
	for(vector<Container2D>::iterator sec=sectors.begin();sec!=sectors.end();++sec) //for all sectors
	{
			
		sec->update();
		for(vector<Circle2D>::iterator it=sec->objects.begin();it<sec->objects.end();++it) //find exiting objects, add to main field
		{
			if(!sec->contains(*it))
			{
				//add to different sector
				Container2D& newSector=findSectorFor(*it);
				it->super=&newSector;
				it->stepBack();

				newSector.addObject(*it);
				//remove from this sector
				if(it==sec->objects.end()-1)
				{
					sec->objects.erase(it);
					break;
				}else{
					it=sec->objects.erase(it);
				}

			}
				
		}
			
	}
}
void TopContainer::draw()
{
	for(vector<Container2D>::iterator it=sectors.begin();it!=sectors.end();++it)
	{
		it->draw();
	}
	for(vector<Circle2D>::iterator it=objects.begin();it!=objects.end();++it)
	{
		it->draw();
	}
}
Container2D& TopContainer::findSectorFor(Circle2D& c)
{
	vector<Container2D>::iterator it;
	for(it=sectors.begin();it!=sectors.end();++it)
	{
		if(it->contains(c))
			return *it;
	}

	return *this;	//returns reference to this top level container
}