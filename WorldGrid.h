#pragma once
#include "lineSegs.h"

class WorldGrid
{
public: 
	GeomNode* Create();

	LineSegs lines;

private:
	float xSize , ySize, zSize = 10;
	float gridStep = 10;


};

