#include "StaticObject.h"

StaticObject::StaticObject()
{
	// �A�j���[�V�����Ȃ��\���@�\���쐬
	mSprite = new Sprite();
}

StaticObject::~StaticObject()
{
	delete mSprite;
}
