#pragma once

#include "Model.h"

// ���̃N���X��h�������āA���낢��ȃ��m�����
// �E���f����\������
// �E�����Ă�����Ɉړ�����
// �E�O�����x�N�g����Ԃ��@�\

class GameObject
{

public:
	GameObject();
	~GameObject();
	void Draw();
	virtual void Update(); // �ړ����S��

	Model* GetModel(); // ���̃N���X������Model�I�u�W�F�N�g��Ԃ�

	// �O�����x�N�g����Ԃ��֐�
	DirectX::XMFLOAT3 GetForwardVector();

	// �ړ����x
	float mSpeed;

private:
	Model* mModel; // ���̃N���X�ŕ\�����郂�f��

	// �O�����x�N�g����ۑ����Ă���
	DirectX::XMFLOAT3 mForwardVector{};

};

