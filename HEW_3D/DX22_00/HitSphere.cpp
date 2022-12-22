#include "HitSphere.h"

bool HitSphere::IsHit(HITBOX obj_1, HITBOX obj_2)
{
	// active‚ªfalse‚È‚ç“–‚½‚è”»’è‚ð‘f’Ê‚è‚³‚¹‚é
	if (obj_1.active == false || obj_2.active == false)
	{
		return false;
	}

	// “–‚½‚è”»’è‚ðŒvŽZ
	float x = obj_2.centerX - obj_1.centerX;
	float y = obj_2.centerY - obj_1.centerY;
	float z = obj_2.centerZ - obj_1.centerZ;
	float ctc = x * x + y * y + z * z; // center to center
	float h1_h2 = obj_2.radius + obj_1.radius;
	return ctc < h1_h2* h1_h2;
}