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

	// �����`�p
	float sizeX;	// ��
	float sizeY;	// ����
	float sizeZ;	// ���s��

	// �~�p
	float radius;	// ���a
};

class HitCheck
{
public:
	// �R���X�g���N�^
	HitCheck();
	~HitCheck();

	// �����蔻������s����֐�
	virtual bool IsHit(HITBOX obj_1, HITBOX obj_2) = 0;

	// makeHitBox
	// �@�\	�FHITBOX�\���̂����
	// ����	�F�\���̂ɂ�����
	// �߂�l	�FHITBOX�\����
	// ����	�F�ۂƊۂ̔����������������radius�ɒl������
	// �@�@	�@�l�p�Ǝl�p�̔����������������width��height�ɒl������
	// �@�@	�@�g��Ȃ��l�ɂ�0.0f������
	HITBOX make_HitBox(DirectX::XMFLOAT3 obj_pos, float obj_radius, float obj_sizeX, float obj_sizeY, float obj_sizeZ);

	// show_HitBox
	// �@�\	�FHITBOX�͈̔͂�`��
	// ����	�F�Ȃ�
	// �߂�l	�F�Ȃ�
	void show_HitBox(void);

private:
	// �q�b�g�{�b�N�X�\���p
	ID3D11ShaderResourceView* m_hitbox;
};

