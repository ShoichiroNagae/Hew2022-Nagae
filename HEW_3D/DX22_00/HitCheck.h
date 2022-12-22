#pragma once

#include <DirectXMath.h>
#include "Direct3D.h"

// �����蔻��͈̓f�[�^�Ǘ��p�\����
struct HITBOX
{
	// �A�N�e�B�u�t���O
	bool active = true;

	// ���S���W
	float centerX;
	float centerY;
	float centerZ;

	// ���a
	float radius;

	// �����`�̃T�C�Y
	float size;

	// �����`�p
	float width;
	float height;
};

class HitCheck
{
public:
	// �����蔻������s����֐�
	virtual bool IsHit(HITBOX obj_1, HITBOX obj_2) = 0;

	// makeHitBox
	// �@�\	�FHITBOX�\���̂����
	// ����	�F�\���̂ɂ�����
	// �߂�l	�FHITBOX�\����
	HITBOX make_HitBox(DirectX::XMFLOAT3 obj_pos, float obj_radius, float obj_size, float obj_width, float obj_height);

private:

};

