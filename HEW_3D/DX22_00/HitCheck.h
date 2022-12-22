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

	// 半径
	float radius;

	// 正方形のサイズ
	float size;

	// 長方形用
	float width;
	float height;
};

class HitCheck
{
public:
	// 当たり判定を実行する関数
	virtual bool IsHit(HITBOX obj_1, HITBOX obj_2) = 0;

	// makeHitBox
	// 機能	：HITBOX構造体を作る
	// 引数	：構造体にある情報
	// 戻り値	：HITBOX構造体
	HITBOX make_HitBox(DirectX::XMFLOAT3 obj_pos, float obj_radius, float obj_size, float obj_width, float obj_height);

private:

};

