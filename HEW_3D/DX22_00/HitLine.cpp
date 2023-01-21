#include "HitLine.h"
using namespace DirectX;

bool HitLine::IsHit(HitSphere * pOther)
{
	// ベクトル①を求める
	// XMFloat3からXMVectorに変換する
	XMVECTOR sphereCenter = XMLoadFloat3(&pOther->mCenter);
	XMVECTOR point1 = XMLoadFloat3(&mPoint1);
	// 引き算でベクトルを計算する
	XMVECTOR vector1 = sphereCenter - point1;

	// ベクトル②を求める
	XMVECTOR point2 = XMLoadFloat3(&mPoint2);
	XMVECTOR vector2 = point2 - point1;

	// ベクトル②を単位ベクトルにする
	XMVECTOR vector2Norm = XMVector3Normalize(vector2);

	// ベクトル①とベクトル②の内積を計算する
	XMVECTOR length1 = XMVector3Dot(vector1, vector2Norm);

	// 垂線と線分の交点を求める
	XMVECTOR intersection =
		point1 + (vector2Norm * XMVectorGetX(length1));

	// 球の中心から交点までの長さを計算する
	XMVECTOR length2 =
		XMVector3Length(intersection - sphereCenter);

	// 計算した長さが球の半径より短ければ衝突しているとなる
	if (XMVectorGetX(length2) <= pOther->mHankei)
	{
		// 直線と球は当たっている
		// 線分の範囲に収まっているかを追加で判断する
		float lengthA, lengthB, lengthC;
		XMVECTOR tmpVec;
		tmpVec = XMVector3Length(point1 - intersection);
		lengthA = XMVectorGetX(tmpVec);

		tmpVec = XMVector3Length(point2 - intersection);
		lengthB = XMVectorGetX(tmpVec);

		tmpVec = XMVector3Length(point2 - point1);
		lengthC = XMVectorGetX(tmpVec);

		float sub = lengthA + lengthB - lengthC;
		sub = fabsf(sub);

		return sub <= 0.1f;
	}
	else
	{
		return false;
	}
}

HitLine::HitLine(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
{
	mPoint1 = p1;
	mPoint2 = p2;
}
