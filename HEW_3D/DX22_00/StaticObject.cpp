#include "StaticObject.h"

StaticObject::StaticObject()
{
	// アニメーションなし表示機能を作成
	mSprite = new Sprite();
}

StaticObject::~StaticObject()
{
	delete mSprite;
}
