#ifndef TEXTURECOLLECTION_H
#define TEXTURECOLLECTION_H
#include <gl/GLAUX.H>
#include <vector>
using namespace::std;
//const int MAX_TX=10;

class TextureCollection
{
	public:
	vector<GLuint> textures;

	int at;
	int period;
	int requested;
	
	TextureCollection():at(0),period(4),requested(0){textures.push_back(0);}
	TextureCollection(GLuint TX):at(0),period(4),requested(0){textures.push_back(TX);}
	//TextureCollection(const TextureCollection& t):at(t.at),period(t.period),requested(t.requested),textures(t.textures){}
	void addTX(GLuint TX){textures.push_back(TX);}

	GLuint getTX();
	void nextTX();

};


#endif