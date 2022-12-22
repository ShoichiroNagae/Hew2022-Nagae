#pragma once
#include <vector>
#include <map>
#include <string>

#include "GameObject.h"
#include "ModelData.h"

typedef std::vector<GameObject*> GameObjVector;

// ゲーム内で使用するVectorをまとめたクラス
class UnifyVec
{
public:
	// GameObjectの描画，更新，解放
	void Vector_DrawAll(GameObjVector vec);
	void Vector_UpdateAll(GameObjVector vec);
	void Vector_DeleteAll(GameObjVector vec);
private:
	GameObjVector gObjList;			// オブジェクトリスト
	GameObjVector gShotManager;		// 弾管理用配列
	GameObjVector gEffectManager;	// エフェクト管理用配列

	std::map<std::string, ModelData> gModelManager;

};

