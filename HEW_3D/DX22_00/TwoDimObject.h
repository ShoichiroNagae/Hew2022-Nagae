#pragma once
#include "Sprite.h"

class TwoDimObject
{
public:
	// �\���@�\
	Sprite* mSprite;

	virtual void Update();
	void Draw();

	// �\��/��\����؂�ւ���ϐ�
	bool active = true;

	virtual void SetActive(bool state); // active�ɒl���Z�b�g����֐�

	// �T�C�Y���Z�b�g����֐�
	void SetSize(float x, float y);
	// �ʒu���Z�b�g����֐�
	void SetPosition(float x, float y);

};

