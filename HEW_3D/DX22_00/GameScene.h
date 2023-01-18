#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>
#include <random>
#include "ObjModelLoader.h"

// マクロ定義
constexpr auto MAX_GROUND = (10);

// プレイヤーの視界内に入るX座標の範囲
#define ENEMY_MIN_XPOS	(-7.0f);	// 最小値(左端)
#define ENEMY_MAX_XPOS	(7.0f);		// 最大値(右端)
#define ENEMY_SPEED_DEF (0.001f);	// スピード　初期値

class GameScene : public BaseScene
{
public:
	GameScene(void);
	~GameScene(void);

	void Update(void);
	void Draw(void);

	void Init(void);
	void Release(void);

	void CreateConstBuffer(void);

	void CameraInit(Camera* cam);

	// 3Dモデルロード
	// 1.ローダー名 2.オブジェクト名 3.objファイル名 4.テクスチャファイル名
	void ModelLoad(ObjModelLoader oml,std::string ModelName,
		const char* pObjFileName, const wchar_t* pTexFileName);

	// 2Dモデルロード
	void ModelLoad(ObjModelLoader oml,std::string ModelName,
		float width, float height, float uvWidth, float uvHeight,const wchar_t* pTexFileName);
	
	// オブジェクト生成
	// 1.オブジェクト名 2.倍率 3,4,5 x,y,z座標
	void ObjectCreate(std::string objName,
		float mScale, float mx, float my, float mz);

protected:

private:
	Camera* gpCamera; // カメラ
	ObjModelLoader loader;	// モデルのローダー

	// モデルマネージャー
	// 名前で格納する 呼び出すときはstring型
	std::map <std::string, ModelData> gModelManager;
	// オブジェクトマネージャー
	std::map <std::string, GameObject*> gObjManager;
	// エネミーマネージャー
	std::vector<GameObject*> gEnemyManager;

	// 銃弾マネージャー
	std::vector<GameObject*> gShotManager;

	//ローダーマネージャー
	std::vector<ObjModelLoader*> Loader;
	
	DWORD gDeltaTime;

	// 地面
	GameObject* gpGround[MAX_GROUND][MAX_GROUND];

	// 敵を自動で生成する関数
	void CreateEnemy();
};