#pragma once
#include <DirectXMath.h>
//�A�j���[�V�����ɕK�v�Ȋe�f�[�^��ێ�����w�b�_�[
// �g���V�[����cpp�ɃC���N���[�h

// ****�@2D�L�����p�@*****
// �r���{�[�h�I�u�W�F�N�g�̃T�C�Y(w,h,uvW,uvH)
DirectX::XMFLOAT4 CHAR2DSize = DirectX::XMFLOAT4(1.0f, 1.2f, 0.33f, 0.25f);
// �A�j���[�V�����̏�ԕϐ��i�c���@�u���W�p�j
enum CHAR2DSTATE
{
	FRONT,
	LEFT,
	RIGHT,
	BACK
};
// �A�j���[�V�����̊�{����   (���� �t���W�p)
// ������-1    ��蒴����ƕ`�悳��Ȃ�
int Char2D_kihonFlame[] = { 0,1,2,1,-1 };


// �K�v���Ƃɒǉ�