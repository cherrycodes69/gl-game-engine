#ifndef MODELCOLLECTION_H
#define MODELCOLLECTION_H

#include <vector>
#include <gl\GLAUX.H>
#include "MilkshapeModel.h"
using namespace::std;
//const int MAX_TX=10;

class ModelCollection
{
	public:
	MilkshapeModel **models;

	int at;
	int period;
	int requested;
	int size;
	
	ModelCollection():at(0),period(4),requested(0),size(0){
		models=new MilkshapeModel*[17];

	}
	
	//TextureCollection(const TextureCollection& t):at(t.at),period(t.period),requested(t.requested),textures(t.textures){}
	void addModel(const char *filename){
		MilkshapeModel *mod=new MilkshapeModel();
		mod->loadModelData(filename);
		models[size++]=mod;
	};

	MilkshapeModel& getModel();
	void nextModel();

	~ModelCollection(){
		for(int i=0;i<size;i++)
			delete models[i];
		delete models;
	}
};


#endif