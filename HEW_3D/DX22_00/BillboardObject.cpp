#include "BillboardObject.h"
#include "BillboardModel.h"

BillboardObject::BillboardObject()
{
	// ビルボード機能ありのModelクラスをnewする
	mModel = new BillboardModel();
	// 当たり判定をnewする
	mHit = new HitSphere(this->mModel->mPos, HITBOX_DEFALT);
}
