#pragma once
#include "HitCheck.h"

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
	void set_Box(HITBOX obj_1, HITBOX obj_2);

	BOX m_box1, m_box2;
};

