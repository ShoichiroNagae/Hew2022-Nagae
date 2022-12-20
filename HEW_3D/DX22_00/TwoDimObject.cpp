#include "TwoDimObject.h"

void TwoDimObject::Update()
{
	mSprite->Update();
}

void TwoDimObject::Draw()
{
	if (active == true)
	{
		mSprite->Draw();
	}
}

void TwoDimObject::SetActive(bool state)
{
	active = state;
}

void TwoDimObject::SetSize(float x, float y)
{
	mSprite->mSizeX = x;
	mSprite->mSizeY = y;
}

void TwoDimObject::SetPosition(float x, float y)
{
	mSprite->mCenterX = x;
	mSprite->mCenterY = y;
}
