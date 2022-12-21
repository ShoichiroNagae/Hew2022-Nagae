#pragma once

#include "Model.h"
#include "HitSphere.h"

// �����蔻��͈̔́@�f�t�H���g
#define HITBOX_DEFALT	(1.0f)

// ���̃N���X���炢�낢��ȃ��m��h���ō��
// �E���f���\���@�\
// �E�����Ă�������ɐi�ދ@�\
// �E�O�����x�N�g����Ԃ��@�\
class GameObject
{

public:
	GameObject();
	~GameObject();
	void Draw();
	virtual void Update(); // �p����ŃI�[�o�[���C�h����O��

	// ���̃N���X�̕\���Ɏg����Model�I�u�W�F�N�g��Ԃ�
	Model* GetModel();
	// �O�����x�N�g����Ԃ��֐�
	DirectX::XMFLOAT3 GetForwardVector();

	// ���̃N���X�̓����蔻�����Ԃ��֐�
	HitSphere* GetHit() { return this->mHit; }

	// �ړ����x
	float mSpeed;

protected:
	// ���̃N���X�ŕ\�������郂�f��
	Model* mModel;

	// ���̃N���X�̓����蔻��
	HitSphere* mHit;

private:
	// �O�����x�N�g����ۑ����Ă����ϐ�
	DirectX::XMFLOAT3 mForwardVector;

};

