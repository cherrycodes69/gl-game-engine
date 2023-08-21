#ifndef TEXTURECOLLECTION_H
#define TEXTURECOLLECTION_H
#include <gl\GLAUX.H>

const int MAX_TX=10;

class TextureCollection
{
	public:
	GLuint textures[MAX_TX];
	int nTX;
	int atTX;
	int periodTX;
	int requestedTX;
	
	TextureCollection(){};
	TextureCollection(GLuint *_textures,int _nTX,int _periodTX);
	GLuint getTX();
	void nextTX();

};


#endif