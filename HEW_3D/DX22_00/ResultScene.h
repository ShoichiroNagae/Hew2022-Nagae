#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "myPolygon.h"
#include "GameObject.h"
#include <map>
#include <string>

class ResultScene : public BaseScene
{
public:
	void Update();
	void Draw();

	ResultScene();
	~ResultScene();

	void CreateConstBuffer();
	void InitCamera(Camera*);
	void Release();
private:
	Camera* gpCamera;
	myPolygon* mPoly;
	// 名前で格納する 呼び出すときはstring型
	std::map <std::string, ModelData> gModelManager;
	// オブジェクトマネージャー
	std::map <std::string, GameObject*> gObjManager;

};

