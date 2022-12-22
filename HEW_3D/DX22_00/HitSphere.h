#pragma once
#include "HitCheck.h"

// 円と円の当たり判定
class HitSphere :public HitCheck
{
public:
	// 引数で指定されたデータを比較する
	bool IsHit(HITBOX obj_1, HITBOX obj_2)override;

private:

};

