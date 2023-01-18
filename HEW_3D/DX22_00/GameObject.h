#pragma once

#include "Model.h"

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

	// �ړ����x
	float mSpeed;

	/// �e�N�X�`���z��(���삪�d���Ȃ����ז�����)
	float animTime = 0.000f;
	int animFlame = 0;
protected:
	// ���̃N���X�ŕ\�������郂�f��
	Model* mModel;

private:
	// �O�����x�N�g����ۑ����Ă����ϐ�
	DirectX::XMFLOAT3 mForwardVector;

};

