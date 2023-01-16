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

	//　ボタンの総数
	static const int BUTTON_AMOUNT = 2;

	// モデルマネージャー
	std::map <std::string, ModelData> gModelManager;
	// オブジェクトマネージャー
	std::map <std::string, GameObject*> gObjManager;
};


struct RESULT_DATA
{
	int MAX_SPEED;	// 最高速度		300km/h以上 [A] : 250km/h以上 [B] : 200km/h以上 [C] 150km/h以上 [D]　それ以下 [E]
	int AVG_SPEED;	// 平均速度		200km/h以上 [A] : 170km/h以上 [B] : 140km/h以上 [C] 100km/h以上 [D]　それ以下 [E]
	int KILL_ENEMY;	// 倒した敵の数	50体以上	[A] : 40体以上 [B] : 30体以上 [C] 20体以上 [D]　それ以下 [E]
	int COMBO;		// コンボ数		25以上 [A] : 20以上 [B] : 15以上 [C] 10以上 [D]　それ以下 [E]
	int LAND_RANK;	// 着地ランク		1 [A] : 2 [B] : 3 [C] 4 [D]　5 [E]
};

