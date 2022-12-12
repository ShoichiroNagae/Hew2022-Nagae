#pragma once

#include "ModelData.h"
#include "Camera.h"
#include <DirectXMath.h>

// 3Dモデルを表示させるクラス
class Model
{

public:
	virtual void Draw(); // 表示

	// このクラスで表示する3Dデータをセットする
	void SetModelData(ModelData model);

	// 仮想世界（ワールド）上の座標
	DirectX::XMFLOAT3 mPos;
	// 表示回転角度
	DirectX::XMFLOAT3 mRotate;
	// 表示拡大縮小率
	DirectX::XMFLOAT3 mScale;

	// 拡大縮小率のxyzを同じ値で設定する
	void SetScale(float newScale);

	// このモデルに適用するカメラオブジェクト
	Camera* mCamera;

protected:
	ModelData mModelData; // 3Dデータ

	// 定数バッファの転送用
	struct ConstBufferData
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

};

