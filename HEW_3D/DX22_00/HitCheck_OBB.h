#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct OBB
{
	XMFLOAT3 c;		// �I�u�W�F�N�g�̒��S���W
	XMFLOAT3 u[3];	// XYZ�̊e���W���̌X����\�������x�N�g��
	XMFLOAT3 e;		// OBB�̊e���W���ɉ����������̔����i���S�_����ʂ܂ł̒����j
};

struct VERTEX
{
	XMFLOAT3 positon, normal;
	float tu, tv;
};

class HitCheck_OBB
{
public:
	void CreateOBB(OBB* obb);

private:

};

