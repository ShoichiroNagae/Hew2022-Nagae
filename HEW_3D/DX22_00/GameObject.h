#pragma once

#include "Model.h"

// このクラスを派生させて、いろいろなモノを作る
// ・モデルを表示する
// ・向いてる方向に移動する
// ・前向きベクトルを返す機能

class GameObject
{

public:
	GameObject();
	~GameObject();
	void Draw();
	virtual void Update(); // 移動も担当

	Model* GetModel(); // このクラスが持つModelオブジェクトを返す

	// 前向きベクトルを返す関数
	DirectX::XMFLOAT3 GetForwardVector();

	// 移動速度
	float mSpeed;

private:
	Model* mModel; // このクラスで表示するモデル

	// 前向きベクトルを保存しておく
	DirectX::XMFLOAT3 mForwardVector{};

};

