#include "NormalObject.h"

NormalObject::NormalObject()
{
	// ビルボードなしModelクラスをnewする
	mModel = new Model();
	// 当たり判定クラスをnewする
	mHit = new HitSphere(mModel->mPos, 1.0f);
}
