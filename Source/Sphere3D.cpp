#include "Object3D.h"

Sphere3D::Sphere3D(Vector3D _pos,Vector3D _dir,double _bindingRadius,Space3D* containingField,TextureCollection _TX,GLUquadricObj *_quadratic,
				  int _rotspeed,double _speed,int _health)
				   :Object3D(_pos,Vector3D(_bindingRadius*2,_bindingRadius*2,_bindingRadius*2),TX),rotspeed(_rotspeed)
{
	bindingRadius=_bindingRadius;
	speed=_speed;
	health=_health;
	quadratic=_quadratic;
	xrot=yrot=20;
	//owners.push_back(containingField);
	TX=_TX;
	health=100;
	dir=_dir;
	dir.normalize();

	
}

void Sphere3D::update()
{
	
	if(health>=0){
	
	detectCollisions();
	forceInBounds();
	stepForward();
	takeDamage();
	
	}
	
};
void Sphere3D::draw()
{
	glBindTexture(GL_TEXTURE_2D,TX.getTX());
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
	glBindTexture(GL_TEXTURE_2D,NULL);

}
void Sphere3D::handleCollision(Object3D& b)
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

bool Sphere3D::intersects(Object3D& b)
{
	double distance = sqrt(pow((pos.dx-b.pos.dx),2)+pow((pos.dy-b.pos.dy),2)+pow((pos.dz-b.pos.dz),2));

	if(distance < bindingRadius + b.bindingRadius )
		return true;
	return false;
}

void Sphere3D::actOn(Object3D& p){
		p.takeDamage();
};