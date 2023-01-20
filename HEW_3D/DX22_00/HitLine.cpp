#include "HitLine.h"
using namespace DirectX;

bool HitLine::IsHit(HitSphere * pOther)
{
	// �x�N�g���@�����߂�
	// XMFloat3����XMVector�ɕϊ�����
	XMVECTOR sphereCenter = XMLoadFloat3(&pOther->mCenter);
	XMVECTOR point1 = XMLoadFloat3(&mPoint1);
	// �����Z�Ńx�N�g�����v�Z����
	XMVECTOR vector1 = sphereCenter - point1;

	// �x�N�g���A�����߂�
	XMVECTOR point2 = XMLoadFloat3(&mPoint2);
	XMVECTOR vector2 = point2 - point1;

	// �x�N�g���A��P�ʃx�N�g���ɂ���
	XMVECTOR vector2Norm = XMVector3Normalize(vector2);

	// �x�N�g���@�ƃx�N�g���A�̓��ς��v�Z����
	XMVECTOR length1 = XMVector3Dot(vector1, vector2Norm);

	// �����Ɛ����̌�_�����߂�
	XMVECTOR intersection =
		point1 + (vector2Norm * XMVectorGetX(length1));

	// ���̒��S�����_�܂ł̒������v�Z����
	XMVECTOR length2 =
		XMVector3Length(intersection - sphereCenter);

	// �v�Z�������������̔��a���Z����ΏՓ˂��Ă���ƂȂ�
	if (XMVectorGetX(length2) <= pOther->mHankei)
	{
		// �����Ƌ��͓������Ă���
		// �����͈̔͂Ɏ��܂��Ă��邩��ǉ��Ŕ��f����
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
