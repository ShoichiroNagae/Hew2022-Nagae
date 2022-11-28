#pragma once

#include "ModelData.h"
#include "Camera.h"
#include <DirectXMath.h>

// 3D���f����\������
class Model
{

public:
	void Draw(); // �\������

	// ���̃N���X�ŕ\������3D�f�[�^���Z�b�g����
	void SetModelData(ModelData model);

	// ���z���E�i���[���h�j��̈ʒu���w�肷����W
	DirectX::XMFLOAT3 mPos;
	// ��]�p�x
	DirectX::XMFLOAT3 mRotate;
	// �g��k����
	DirectX::XMFLOAT3 mScale;

	// �g��k������xyz�ɓ����l�Őݒ肷��
	void SetScale(float newScale);

	// ���̃��f���ɓK�p����J�����I�u�W�F�N�g
	Camera* mCamera;

private:
	// 3D�f�[�^�����ϐ�
	ModelData mModelData;

};

