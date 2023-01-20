#pragma once

#include "HitSphere.h"

class HitLine
{
public:
	// HitSphere‚Æ‚±‚ÌHitLine‚ªÕ“Ë‚µ‚Ä‚¢‚é‚©”»’è‚·‚é
	bool IsHit(HitSphere* pOther);

	HitLine(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);


private:
	//@ü•ª‚Ì‹´‚ÌÀ•W‚Q‚Â
	DirectX::XMFLOAT3 mPoint1, mPoint2;
};

