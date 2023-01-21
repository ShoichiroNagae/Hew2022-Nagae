#include "BillboardObject.h"
#include "BillboardModel.h"

BillboardObject::BillboardObject()
{
	// ビルボード機能ありのModelクラスをnewする
	mModel = new BillboardModel();
	// 当たり判定クラスをnewする
	mHit = new HitSphere(mModel->mPos, 1.0f);
}
