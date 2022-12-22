#pragma once

#include "ModelData.h"
#include "Direct3D.h"

class myPolygon
{
public:
	ModelData CreateSquarePolygon(float width, float height,
		float uvWidth, float uvHeight, const wchar_t* texFile);
};

