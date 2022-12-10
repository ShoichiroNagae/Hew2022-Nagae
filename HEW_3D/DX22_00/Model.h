#pragma once

#include "ModelData.h"
#include "Camera.h"
#include <DirectXMath.h>

// 3D���f����\��������N���X
class Model
{

public:
	virtual void Draw(); // �\��

	// ���̃N���X�ŕ\������3D�f�[�^���Z�b�g����
	void SetModelData(ModelData model);

	// ���z���E�i���[���h�j��̍��W
	DirectX::XMFLOAT3 mPos;
	// �\����]�p�x
	DirectX::XMFLOAT3 mRotate;
	// �\���g��k����
	DirectX::XMFLOAT3 mScale;

	// �g��k������xyz�𓯂��l�Őݒ肷��
	void SetScale(float newScale);

	// ���̃��f���ɓK�p����J�����I�u�W�F�N�g
	Camera* mCamera;

protected:
	ModelData mModelData; // 3D�f�[�^

	// �萔�o�b�t�@�̓]���p
	struct ConstBufferData
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

};

