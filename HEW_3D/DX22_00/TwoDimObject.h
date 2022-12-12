#pragma once
#include "Sprite.h"

class TwoDimObject
{
public:
	// 表示機能
	Sprite* mSprite;

	virtual void Update();
	void Draw();

	// 表示/非表示を切り替える変数
	bool active = true;

	virtual void SetActive(bool state); // activeに値をセットする関数

	// サイズをセットする関数
	void SetSize(float x, float y);
	// 位置をセットする関数
	void SetPosition(float x, float y);

};

