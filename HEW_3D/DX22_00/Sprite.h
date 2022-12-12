#pragma once

#include "Direct3d.h"

class Sprite
{
public:
	Sprite(); 
	virtual void Update(); // �����[�v���_�̌v�Z�s��
	void Draw(); // �`��̐ݒ�ƕ`��
	~Sprite();  // �f�X�g���N�^����������s��

	// �ǂݍ��ݍς݂̃e�N�X�`�����󂯎��
	void SetTexture(ID3D11ShaderResourceView* pTexture);

	ID3D11ShaderResourceView* mTexture; // ���̃X�v���C�g�Ŏg���e�N�X�`��
	ID3D11Buffer* mVertexBuffer; // ���̃X�v���C�g�Ŏg�����_�o�b�t�@

	float mCenterX, mCenterY; // ���̃X�v���C�g�̒��S���W
	float mSizeX, mSizeY;      // ���̃X�v���C�g�̃T�C�Y

	// �����N���X
	// �F��\���\����
	struct RGBA
	{
		float r, g, b, a;
	};

	// �|���S���̒��_�p�̍\����
	struct VERTEX2D  // VERTEX = ���_
	{
		float x, y;  // ���_�̍��W�i�ʒu�j
		float u, v;  // �e�N�X�`����UV���W
		RGBA color;  // ���_�̐F
	};

	// ���_�̐F�������Ă����ϐ�
	RGBA mColor;

	// ���_�̐F��ݒ肷��֐�
	void SetColor(RGBA color);

};

