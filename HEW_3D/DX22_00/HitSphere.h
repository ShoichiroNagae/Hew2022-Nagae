#pragma once
#include "HitCheck.h"

// �~�Ɖ~�̓����蔻��
class HitSphere :public HitCheck
{
public:
	// �����Ŏw�肳�ꂽ�f�[�^���r����
	bool IsHit(HITBOX obj_1, HITBOX obj_2)override;

private:

};

