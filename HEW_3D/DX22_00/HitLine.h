#pragma once

#include "HitSphere.h"

class HitLine
{
public:
	// HitSphere�Ƃ���HitLine���Փ˂��Ă��邩���肷��
	bool IsHit(HitSphere* pOther);

	HitLine(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);


private:
	//�@�����̋��̍��W�Q��
	DirectX::XMFLOAT3 mPoint1, mPoint2;
};

