#include "NormalObject.h"

NormalObject::NormalObject()
{
	// �r���{�[�h�Ȃ�Model�N���X��new����
	mModel = new Model();
	// �����蔻���new����
	mHit = new HitSphere(this->mModel->mPos, HITBOX_DEFALT);
}
