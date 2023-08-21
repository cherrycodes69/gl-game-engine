#include "Object3D.h"
Space3D::Space3D(Vector3D _pos,Vector3D _dim,TextureCollection _TX,Space3D *_super, bool borders)
			:Object3D(_pos,_dim,_TX){
	TX=_TX;
	addOwner(_super);
	draw_borders=borders;
		
};
bool Space3D::containsOriginOf(Object3D& o){
		if(o.center()>center()-(dim*0.5) && o.center()<center()+(dim*0.5))
			return true;
		return false;};

void Space3D::addObject(Object3D* o)
{
	//o->super=this;
	o->addOwner(this);
	objects.push_back(o);
}

void Space3D::update()
{
	for(list<Object3D*>::iterator it=objects.begin();it!=objects.end();++it)
			(*it)->update();
}

Space3D::~Space3D()
{


}

void Space3D::draw()
{
	for(list<Object3D*>::iterator it=objects.begin();it!=objects.end();++it)
	{
		(*it)->draw();
	}
	if(draw_borders){
		double dl=dim.dx/2;
		double dh=dim.dy/2;
		double dd=dim.dz/2;

		glBindTexture(GL_TEXTURE_2D,TX.textures[0]);
		glPushMatrix();
		glTranslatef(pos.dx+dl, pos.dy+dh, pos.dz+dd);
		glNormal3f(0,0,-1);
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f, 0.0f);glVertex3f( dl, dh, dd); //up right corner
		glTexCoord2f(1.0f, 0.0f);glVertex3f( -dl, dh, dd); //up left corner
		glTexCoord2f(1.0f, 1.0f);glVertex3f(-dl,-dh, dd); //down left
		glTexCoord2f(0.0f, 1.0f);glVertex3f( dl,-dh, dd);//down right
		glEnd();
		glNormal3f(0,0,1);
		glBegin(GL_QUADS); 
		glTexCoord2f(1.0f, 0.0f);glVertex3f( dl, dh, -dd); //up right corner
		glTexCoord2f(1.0f, 1.0f);glVertex3f( -dl, dh, -dd); //up left corner
		glTexCoord2f(0.0f, 1.0f);glVertex3f(-dl,-dh, -dd); //down left
		glTexCoord2f(0.0f, 0.0f);glVertex3f( dl,-dh, -dd);//down right
		glEnd();
		glNormal3f(0,-1,0);
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f, 1.0f);glVertex3f( dl, dh, dd); //up right corner
		glTexCoord2f(0.0f, 0.0f);glVertex3f( -dl, dh, dd); //up left corner
		glTexCoord2f(1.0f, 0.0f);glVertex3f(-dl,dh, -dd); //down left
		glTexCoord2f(1.0f, 1.0f);glVertex3f( dl,dh, -dd);//down right
		glEnd();
		glBindTexture(GL_TEXTURE_2D,TX.textures[1]);
		glNormal3f(0,1,0);
		glBegin(GL_QUADS); 
		glTexCoord2f(1.0f, 1.0f);glVertex3f( dl, -dh, dd); //up right corner
		glTexCoord2f(0.0f, 1.0f);glVertex3f( -dl, -dh, dd); //up left corner
		glTexCoord2f(0.0f, 0.0f);glVertex3f(-dl,-dh, -dd); //down left
		glTexCoord2f(1.0f, 0.0f);glVertex3f( dl,-dh, -dd);//down right
		glEnd();
		glBindTexture(GL_TEXTURE_2D,TX.textures[0]);
		glNormal3f(-1,0,0);
		glBegin(GL_QUADS); 
		glTexCoord2f(1.0f, 0.0f);glVertex3f( dl, -dh, -dd); //up right corner
		glTexCoord2f(1.0f, 1.0f);glVertex3f( dl, -dh, dd); //up left corner
		glTexCoord2f(0.0f, 1.0f);glVertex3f(dl,dh, dd); //down left
		glTexCoord2f(0.0f, 0.0f);glVertex3f( dl,dh, -dd);//down right
		glEnd();
		glNormal3f(1,0,0);
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f, 0.0f);glVertex3f( -dl, -dh, -dd); //up right corner
		glTexCoord2f(1.0f, 0.0f);glVertex3f( -dl, -dh, dd); //up left corner
		glTexCoord2f(1.0f, 1.0f);glVertex3f(-dl,dh, dd); //down left
		glTexCoord2f(0.0f, 1.0f);glVertex3f( -dl,dh, -dd);//down right
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D,0);
	}
};