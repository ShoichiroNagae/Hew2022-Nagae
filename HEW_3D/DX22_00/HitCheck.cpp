#include "HitCheck.h"

HitCheck::HitCheck()
{
}

HitCheck::~HitCheck()
{
}

HITBOX HitCheck::make_HitBox(DirectX::XMFLOAT3 obj_pos, float obj_radius, float obj_sizeX, float obj_sizeY, float obj_sizeZ)
{
	HITBOX tmp = { NULL };

	// データをセット
	tmp.active = true;
	tmp.centerX = obj_pos.x;
	tmp.centerY = obj_pos.y;
	tmp.centerZ = obj_pos.z;
	tmp.sizeX = obj_sizeX;
	tmp.sizeY = obj_sizeY;
	tmp.sizeZ = obj_sizeZ;
	tmp.radius = obj_radius;

	return tmp;
}

void HitCheck::show_HitBox(void)
{
	// ヒットボックス見えるようにしたい
}
