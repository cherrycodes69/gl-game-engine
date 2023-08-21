#include "TextureCollection.h"



GLuint TextureCollection::getTX()
{
	if(textures.size()==0)
		return 0;
	requested++;
	if(requested > period)
	{
		nextTX();
	}
	return textures[at];
}

void TextureCollection::nextTX()
{
	at++;
	at%=textures.size();
	requested=0;
}