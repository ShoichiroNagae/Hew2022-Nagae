#pragma once
#include "BaseScene.h"
#include "myPolygon.h"
#include "ModelData.h"
#include "GameObject.h"

#include <map>
#include <string>
#include <vector>

class TitleScene : public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Update();
	void Draw();

private:
	myPolygon* mPoly{};

	// モデルマネージャー
	std::map <std::string, ModelData> gModelManager;
	std::vector<GameObject*> gObjectList;


};

