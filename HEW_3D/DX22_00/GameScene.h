#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>
#include "ObjModelLoader.h"

// マクロ定義
constexpr auto MAX_GROUND = (10);

class GameScene : public BaseScene
{
public:
	GameScene(void);
	~GameScene(void);

	void Update(void);
	void Draw(void);

	void Init(void);
	void Release(void);

	void CreateConstBudder(void);

	void CameraInit(Camera* cam);

	// 3Dモデルロード
	// 1.ローダー名 2.オブジェクト名 3.objファイル名 4.テクスチャファイル名
	void ModelLoad(ObjModelLoader oml,std::string ModelName,
		const char* pObjFileName, const wchar_t* pTexFileName);

protected:

private:
	Camera* gpCamera; // カメラ
	ObjModelLoader loader;	// モデルのローダー

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