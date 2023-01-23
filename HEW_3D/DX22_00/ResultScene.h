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
	// ���O�Ŋi�[���� �Ăяo���Ƃ���string�^
	std::map <std::string, ModelData> gModelManager;
	// �I�u�W�F�N�g�}�l�[�W���[
	std::map <std::string, GameObject*> gObjManager;

};

