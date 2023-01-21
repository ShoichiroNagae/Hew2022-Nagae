#pragma once

#include "Model.h"
#include "HitSphere.h"

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

	// �����蔻��I�u�W�F�N�g getter setter
	HitSphere* GetHit()
	{
		return mHit;
	}

	// �ړ����x
	float mSpeed;

	// �A�N�e�B�u�t���O
	float mActive;

protected:
	// ���̃N���X�ŕ\�������郂�f��
	Model* mModel;

	HitSphere* mHit; // �����蔻��

private:
	// �O�����x�N�g����ۑ����Ă����ϐ�
	DirectX::XMFLOAT3 mForwardVector;

};

