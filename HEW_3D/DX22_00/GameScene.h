#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>
#include <random>
#include "ObjModelLoader.h"

// マクロ定義
constexpr auto MAX_GROUND = (100);

// プレイヤーの視界内に入るX座標の範囲
#define ENEMY_MIN_XPOS	(-5.0f);	// 最小値(左端)
#define ENEMY_MAX_XPOS	(3.0f);		// 最大値(右端)
#define ENEMY_SPEED_DEF (0.001f);	// スピード　初期値

#define GROUND_POS	(400.0f)

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
	
	// ビルボードアニメーション関係系用ローダー　　作成::井戸上
	void ModelLoad(ObjModelLoader oml, std::string ModelName,
		DirectX::XMFLOAT4 Set_whuv, const wchar_t* pTexFileName);

	// オブジェクト生成
	// 1.オブジェクト名 2.倍率 3,4,5 x,y,z座標
	void ObjectCreate(std::string objName,
		float mScale, float mx, float my, float mz);

protected:

private:
	Camera*				gpCamera;			// カメラ
	ObjModelLoader		loader;				// モデルのローダー

	int					frameCount;			// フレームカウント
	int					frameCountChange;	// リザルトシーンへ遷移する時のカウント
	int					nowCombo;			// 現在のコンボ数
	int					maxCombo;			// 最大コンボ数
	bool				playerLanded;		// プレイヤーが地面についたかどうか

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
	std::vector<GameObject*> gGround;

	// 敵を自動で生成する関数
	void CreateEnemy();
	// 敵を消してもいいか判定する関数
	bool CheckEnemy(GameObject* _enemy);
	// プレイヤーの移動範囲を制限する
	void MoveLimit();
	// 攻撃アニメーションか
	int SetAnimState = 0;
};