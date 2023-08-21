#include "Object3D.h"
bool assigned;
SuperSpace3D::SuperSpace3D(int _n,Vector3D _dim,TextureCollection _TX,bool db=false)
	:Space3D(Vector3D(0,0,0)-_dim*0.5,_dim,_TX,0,0)
	{
		//glLoadIdentity();
		n=_n;
		  TX=_TX;
		 draw_borders=!db;
		lStep=dim.dx/n;
		hStep=dim.dx/n;
		dStep=dim.dx/n;
		
		int i=0,j=0,k=0;
	
		for( i=0;i<n;++i)
			for( j=0;j<n;++j)
				for( k=0;k<n;++k)
					sectors.push_back(Space3D(Vector3D(pos.dx+i*lStep,pos.dy+j*hStep,pos.dz+k*dStep),Vector3D(lStep,hStep,dStep),TX,this,db));
	
		//sectors.push_back(Space3D(originX+hStep*i-hStep/8,originY+vStep*j-vStep/8,hStep*1.25,vStep*1.25));
		dim.dx=i*lStep;
		dim.dy=j*hStep;
		dim.dz=k*dStep;


	};

void SuperSpace3D::update()
{
	bool reassign=false;
	for(list<Object3D*>::iterator it=getObjects().begin();it!=getObjects().end() ;it++) //for all objects in field, distribute to sectors
	{
		if(!(*it)->dead()){
			
			for(int i=1;i<(*it)->owners.size();++i)
				if((*it)->owners[i]->intersects(**it))
					reassign=true;
			if(reassign || (*it)->owners.size()<=1)
			{
				assignSectors(**it);
			}
			(*it)->update();

		}
	}
}
void SuperSpace3D::draw()
{
	
	Space3D::draw();
	
	for(vector<Space3D>::iterator it=sectors.begin();it!=sectors.end();++it)
	{
		it->draw();
	}
}
int SuperSpace3D::getSectorIndex(Vector3D p)
{
	int xI=abs(p.dx-pos.dx)/lStep;
	int yI=abs(p.dy-pos.dy)/hStep;
	int zI=abs(p.dz-pos.dz)/dStep;
	
	
	if(xI>=n) xI=n-1;
	if( yI>=n ) yI=n-1;
	if( zI>=n) zI=n-1;

	int index=xI*n*n+yI*n+zI;
	return index;
}

void SuperSpace3D::assignSectors(Object3D& c)
{
	c.owners.erase(c.owners.begin()+1,c.owners.end());

	if(c.getDir().dx!=c.getDir().dx ){//|| c.dir.dy!=c.dir.dy || c.dir.dz!=c.dir.dz || c.pos.dx!=c.dir.dx || c.pos.dy!=c.dir.dy || c.pos.dz!=c.dir.dz){ //#ind
        c.getDir()=Vector3D(0,0,0);
		c.getPos()=Vector3D(0,0,0);
		addObject(&c);
		return;
	}
	
	list<int> indecies;
	int index;

	Vector3D cmax(c.center()+(c.getDim()*0.5));
	index=getSectorIndex(cmax);
	indecies.push_back(index);
	Vector3D tmp(cmax);
	tmp.dx-=c.getDim().dx;
	index=getSectorIndex(tmp);
	indecies.push_back(index);
	tmp=cmax;
	tmp.dy-=c.getDim().dy;
	index=getSectorIndex(tmp);
	indecies.push_back(index);
	tmp=cmax;
	tmp.dz-=c.getDim().dz;
	index=getSectorIndex(tmp);
	indecies.push_back(index);


	Vector3D cmin(c.center()-(c.getDim()*0.5));
	index=getSectorIndex(tmp);
	indecies.push_back(index);
	tmp=cmin;
	tmp.dy+=c.getDim().dx;
	index=getSectorIndex(tmp);
	indecies.push_back(index);
	tmp=cmin;
	tmp.dy+=c.getDim().dy;
	index=getSectorIndex(tmp);
	indecies.push_back(index);
	tmp=cmin;
	tmp.dy+=c.getDim().dz;
	index=getSectorIndex(tmp);
	indecies.push_back(index);
	
	indecies.unique();
	
	for(list<int>::iterator it=indecies.begin();it!=indecies.end();++it){
		sectors[*it].addObject(&c);
		sectors[*it].getObjects().unique();
	}
	
	//sectors[getSectorIndex(c.center())].addObject(&c);

}