#include "HitCheck.h"

HITBOX HitCheck::make_HitBox(DirectX::XMFLOAT3 obj_pos, float obj_radius, float obj_size, float obj_width, float obj_height)
{
	HITBOX tmp = { NULL };

	// データをセット
	tmp.active = true;
	tmp.centerX = obj_pos.x;
	tmp.centerY = obj_pos.y;
	tmp.centerZ = obj_pos.z;
	tmp.radius = obj_radius;
	tmp.size = obj_size;
	tmp.width = obj_width;
	tmp.height = obj_height;

	return tmp;
}
