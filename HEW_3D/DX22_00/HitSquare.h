#pragma once
#include "HitCheck.h"

// AABB�����蔻��p�\����
struct BOX
{
	float xMax, xMin;
	float yMax, yMin;
	float zMax, zMin;
};

// �l�p�Ǝl�p�̓����蔻��
// AABB
class HitSquare :public HitCheck
{
public:
	// �l�p�Ǝl�p�̓����蔻��
	bool IsHit(HITBOX obj_1, HITBOX obj_2)override;

private:
	// AABB�����蔻��p�\���̂Ƀf�[�^���Ԃ�����
	void set_Box(HITBOX obj_1, HITBOX obj_2);

	// �����蔻��p�f�[�^�i�[�p�ϐ�
	BOX m_box1, m_box2;	
};

