#pragma once

#include "Model.h"
#include "HitSphere.h"

// このクラスからいろいろなモノを派生で作る
// ・モデル表示機能
// ・向いている方向に進む機能
// ・前向きベクトルを返す機能
class GameObject
{ 
public:
	GameObject();
	~GameObject();
	void Draw();
	virtual void Update(); // 継承先でオーバーライドする前提

	// このクラスの表示に使われるModelオブジェクトを返す
	Model* GetModel();

	// 前向きベクトルを返す関数
	DirectX::XMFLOAT3 GetForwardVector();

	// 当たり判定オブジェクト getter setter
	HitSphere* GetHit()
	{
		return mHit;
	}

	// 移動速度
	float mSpeed;

	// アクティブフラグ
	float mActive;

protected:
	// このクラスで表示させるモデル
	Model* mModel;

	HitSphere* mHit; // 当たり判定

private:
	// 前向きベクトルを保存しておく変数
	DirectX::XMFLOAT3 mForwardVector;

};

