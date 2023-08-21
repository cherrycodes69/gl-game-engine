#include "TextureCollection.h"

TextureCollection::TextureCollection(GLuint *_textures,int _nTX,int _periodTX):nTX(_nTX),atTX(0),periodTX(_periodTX),requestedTX(0)
{	
	for(int i=0;i<nTX && nTX < MAX_TX;i++)
		textures[i]=_textures[i];

}

GLuint TextureCollection::getTX()
{
	if(nTX==0)
		return 0;
	requestedTX++;
	if(requestedTX > periodTX)
	{
	//	nextTX();
	}
	return textures[atTX];
}

void TextureCollection::nextTX()
{
	atTX++;
	atTX%=nTX;
	requestedTX=0;
}