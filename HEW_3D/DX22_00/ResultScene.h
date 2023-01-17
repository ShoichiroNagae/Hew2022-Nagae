#pragma once
#include "BaseScene.h"
#include "ModelData.h"
#include "GameObject.h"
#include "Camera.h"

#include <map>
#include <string>
#include "myPolygon.h"

class ResultScene : public BaseScene
{
public:
	ResultScene();
	~ResultScene();

	void Update();
	void Draw();

	void InitCamera(Camera*);
	void CreateConstBuffer();
	void Release();

private:
	Camera* gpCamera;
	myPolygon* mPoly{};

	std::map <std::string, ModelData> gModelManager;
	std::map <std::string, GameObject*> gObjManager;
};

