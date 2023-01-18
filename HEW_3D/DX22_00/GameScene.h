#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>
#include "ObjModelLoader.h"

using namespace DirectX; // "DirectX"namespace������ȗ�����

// �}�N����`
constexpr auto MAX_GROUND = (10);


class GameScene : public BaseScene
{
public:
	GameScene();
	~GameScene();

	void Update();
	void Draw();

	void Init();
	void Release();

	// �r���{�[�h�I�u�W�F�N�g�̃T�C�Y(w,h,uvW,uvH)
	XMFLOAT4 CHAR2DSize = XMFLOAT4(1.0f, 1.2f, 0.33f, 0.25f);
protected:

private:
	Camera* gpCamera; // �J����

	// ���f���}�l�[�W���[
	// ���O�Ŋi�[���� �Ăяo���Ƃ���string�^
	std::map <std::string, ModelData> gModelManager;
	// �I�u�W�F�N�g�}�l�[�W���[
	std::map <std::string, GameObject*> gObjManager;
	// �e�e�}�l�[�W���[
	std::vector<GameObject*> gShotManager;

	//���[�_�[�}�l�[�W���[
	std::vector<ObjModelLoader*> Loader;
	
	DWORD gDeltaTime;

	GameObject* gpGround[MAX_GROUND][MAX_GROUND];
};