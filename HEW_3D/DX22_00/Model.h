#pragma once

#include "ModelData.h"
#include "Camera.h"
#include <DirectXMath.h>

// 3Dモデルを表示する
class Model
{

public:
	void Draw(); // 表示する

	// このクラスで表示する3Dデータをセットする
	void SetModelData(ModelData model);

	// 仮想世界（ワールド）上の位置を指定する座標
	DirectX::XMFLOAT3 mPos;
	// 回転角度
	DirectX::XMFLOAT3 mRotate;
	// 拡大縮小率
	DirectX::XMFLOAT3 mScale;

	// 拡大縮小率をxyzに同じ値で設定する
	void SetScale(float newScale);

	// このモデルに適用するカメラオブジェクト
	Camera* mCamera;

private:
	// 3Dデータを持つ変数
	ModelData mModelData;

};

