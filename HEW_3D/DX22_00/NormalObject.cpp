#include "NormalObject.h"

NormalObject::NormalObject()
{
	// ビルボードなしModelクラスをnewする
	mModel = new Model();
	// 当たり判定をnewする
	mHit = new HitSphere(this->mModel->mPos, HITBOX_DEFALT);
}
