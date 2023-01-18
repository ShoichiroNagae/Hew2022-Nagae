#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>
#include "ObjModelLoader.h"

using namespace DirectX; // "DirectX"namespaceを今後省略する

// マクロ定義
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

	// ビルボードオブジェクトのサイズ(w,h,uvW,uvH)
	XMFLOAT4 CHAR2DSize = XMFLOAT4(1.0f, 1.2f, 0.33f, 0.25f);
protected:

private:
	Camera* gpCamera; // カメラ

	// モデルマネージャー
	// 名前で格納する 呼び出すときはstring型
	std::map <std::string, ModelData> gModelManager;
	// オブジェクトマネージャー
	std::map <std::string, GameObject*> gObjManager;
	// 銃弾マネージャー
	std::vector<GameObject*> gShotManager;

	//ローダーマネージャー
	std::vector<ObjModelLoader*> Loader;
	
	DWORD gDeltaTime;

	GameObject* gpGround[MAX_GROUND][MAX_GROUND];
};