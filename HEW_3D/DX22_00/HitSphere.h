#pragma once
#include "HitCheck.h"

// �~�Ɖ~�̓����蔻��
class HitSphere :public HitCheck
{
public:
	// �~�Ɖ~�̓����蔻������s����
	bool IsHit(HITBOX obj_1, HITBOX obj_2)override;

private:

};

