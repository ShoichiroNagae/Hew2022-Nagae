#include "BillboardObject.h"
#include "BillboardModel.h"

BillboardObject::BillboardObject()
{
	// �r���{�[�h�@�\�����Model�N���X��new����
	mModel = new BillboardModel();
	// �����蔻���new����
	mHit = new HitSphere(this->mModel->mPos, HITBOX_DEFALT);
}
