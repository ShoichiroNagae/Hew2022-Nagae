#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>

// マクロ定義
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
	Camera* gpCamera; // カメラ
	// モデルマネージャー
	std::map<std::string, ModelData> gModelManager;
	// オブジェクトマネージャー
	std::map <std::string, GameObject*> gObjManager;
	// 銃弾マネージャー
	std::vector<GameObject*> gShotManager;
	DWORD gDeltaTime;

	GameObject* gpGround1[MAX_GROUND];
	GameObject* gpGround2[MAX_GROUND];
	GameObject* gpGround3[MAX_GROUND];
	GameObject* gpGround4[MAX_GROUND];
};