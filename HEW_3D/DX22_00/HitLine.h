#pragma once

#include "HitSphere.h"

class HitLine
{
public:
	// HitSphereとこのHitLineが衝突しているか判定する
	bool IsHit(HitSphere* pOther);

	HitLine(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);


private:
	//　線分の橋の座標２つ
	DirectX::XMFLOAT3 mPoint1, mPoint2;
};

