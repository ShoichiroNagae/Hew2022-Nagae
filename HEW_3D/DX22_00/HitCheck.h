#pragma once

#include <DirectXMath.h>
#include "Direct3D.h"

// 当たり判定範囲データ管理用構造体
struct HITBOX
{
	// アクティブフラグ
	bool active = true;

	// 中心座標
	float centerX;
	float centerY;
	float centerZ;

	// 長方形用
	float sizeX;	// 幅
	float sizeY;	// 高さ
	float sizeZ;	// 奥行き

	// 円用
	float radius;	// 半径
};

class HitCheck
{
public:
	// コンストラクタ
	HitCheck();
	~HitCheck();

	// 当たり判定を実行する関数
	virtual bool IsHit(HITBOX obj_1, HITBOX obj_2) = 0;

	// makeHitBox
	// 機能	：HITBOX構造体を作る
	// 引数	：構造体にある情報
	// 戻り値	：HITBOX構造体
	// メモ	：丸と丸の判定をしたかったらradiusに値を入れる
	// 　　	　四角と四角の判定をしたかったらwidthとheightに値を入れる
	// 　　	　使わない値には0.0fを入れる
	HITBOX make_HitBox(DirectX::XMFLOAT3 obj_pos, float obj_radius, float obj_sizeX, float obj_sizeY, float obj_sizeZ);

	// show_HitBox
	// 機能	：HITBOXの範囲を描画
	// 引数	：なし
	// 戻り値	：なし
	void show_HitBox(void);

private:
	// ヒットボックス表示用
	ID3D11ShaderResourceView* m_hitbox;
};

