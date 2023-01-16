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


struct RESULT_DATA
{
	int MAX_SPEED;	// �ō����x		300km/h�ȏ� [A] : 250km/h�ȏ� [B] : 200km/h�ȏ� [C] 150km/h�ȏ� [D]�@����ȉ� [E]
	int AVG_SPEED;	// ���ϑ��x		200km/h�ȏ� [A] : 170km/h�ȏ� [B] : 140km/h�ȏ� [C] 100km/h�ȏ� [D]�@����ȉ� [E]
	int KILL_ENEMY;	// �|�����G�̐�	50�̈ȏ�	[A] : 40�̈ȏ� [B] : 30�̈ȏ� [C] 20�̈ȏ� [D]�@����ȉ� [E]
	int COMBO;		// �R���{��		25�ȏ� [A] : 20�ȏ� [B] : 15�ȏ� [C] 10�ȏ� [D]�@����ȉ� [E]
	int LAND_RANK;	// ���n�����N		1 [A] : 2 [B] : 3 [C] 4 [D]�@5 [E]
};

