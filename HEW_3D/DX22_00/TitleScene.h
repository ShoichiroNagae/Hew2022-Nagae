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

	void Init();
	void InitCamera(Camera* cam);
	void CreateConstBuffer();
	void ObjectCreate();

	void Release();

private:
	myPolygon* mPoly{};
	Camera* gpCamera;
	int selectNum;

	//�@�{�^���̑���
	static const int BUTTON_AMOUNT = 2;

	// ���f���}�l�[�W���[
	std::map <std::string, ModelData> gModelManager;
	// �I�u�W�F�N�g�}�l�[�W���[
	std::map <std::string, GameObject*> gObjManager;
};