#include "BillboardObject.h"
#include "BillboardModel.h"

BillboardObject::BillboardObject()
{
	// �r���{�[�h�@�\�����Model�N���X��new����
	mModel = new BillboardModel();
	// �����蔻��N���X��new����
	mHit = new HitSphere(mModel->mPos, 1.0f);
}
