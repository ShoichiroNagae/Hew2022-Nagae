#include "NormalObject.h"

NormalObject::NormalObject()
{
	// �r���{�[�h�Ȃ�Model�N���X��new����
	mModel = new Model();
	// �����蔻��N���X��new����
	mHit = new HitSphere(mModel->mPos, 1.0f);
}
