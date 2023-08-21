#include "ModelCollection.h"



MilkshapeModel &ModelCollection::getModel()
{
	if(size==0)
		return MilkshapeModel();
	requested++;
	if(requested > period)
	{
		nextModel();
	}
	return *models[at];
}

void ModelCollection::nextModel()
{
	at++;
	at%=size;
	requested=0;
}