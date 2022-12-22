#pragma once
#include "HitCheck.h"

// AABB当たり判定用構造体
struct BOX
{
	float xMax, xMin;
	float yMax, yMin;
	float zMax, zMin;
};

// 四角と四角の当たり判定
// AABB
class HitSquare :public HitCheck
{
public:
	// 四角と四角の当たり判定
	bool IsHit(HITBOX obj_1, HITBOX obj_2)override;

private:
	// AABB当たり判定用構造体にデータをぶちこむ
	void set_Box(HITBOX obj_1, HITBOX obj_2);

	// 当たり判定用データ格納用変数
	BOX m_box1, m_box2;	
};

