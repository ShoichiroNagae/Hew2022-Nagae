#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>

// �}�N����`
constexpr auto MAX_GROUND = (50);

class GameScene : public BaseScene
{
public:
	GameScene();
	~GameScene();

	void Update();
	void Draw();

	void Init();
	void Release();

protected:

private:
	Camera* gpCamera; // �J����
	// ���f���}�l�[�W���[
	std::map<std::string, ModelData> gModelManager;
	// �I�u�W�F�N�g�}�l�[�W���[
	std::map <std::string, GameObject*> gObjManager;
	// �e�e�}�l�[�W���[
	std::vector<GameObject*> gShotManager;
	DWORD gDeltaTime;

	GameObject* gpGround1[MAX_GROUND];
	GameObject* gpGround2[MAX_GROUND];
	GameObject* gpGround3[MAX_GROUND];
	GameObject* gpGround4[MAX_GROUND];
};