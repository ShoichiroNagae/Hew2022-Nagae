#include "Model.h"
#include "Direct3D.h"

using namespace DirectX; // "DirectX::"������ȍ~�ȗ��ł���

struct ConstBufferData
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};


// �萔�o�b�t�@�p�ϐ�
ID3D11Buffer* gpConstBuffer;

void Model::Draw()
{
	DIRECT3D* d3d = Direct3D_Get();

	// ��]�s��
	static float ry;
	XMMATRIX mxRotY = XMMatrixRotationY(
		XMConvertToRadians(mRotate.y));		// Y����]
	XMMATRIX mxRotX = XMMatrixRotationX(
		XMConvertToRadians(mRotate.x));		// X����] 
	XMMATRIX mxRotZ = XMMatrixRotationZ(
		XMConvertToRadians(mRotate.z));		// Z����]

	// ���s�ړ��s��
	XMMATRIX mxTrans = XMMatrixTranslation(
		mPos.x, mPos.y, mPos.z);  // ���̂̌��݈ʒu��\���ϐ����g���āA���s�ړ��̍s������

	// �g��k���s��
	XMMATRIX mxScale = XMMatrixScaling(
		mScale.x, mScale.y, mScale.z);

	// ���[���h�ϊ��s����쐬
	XMMATRIX mxWorld = mxScale * mxRotZ * mxRotX * mxRotY * mxTrans;

	// �v���W�F�N�V�����ϊ��s����쐬
	// 1: ����p
	// 2: �A�X�y�N�g��@���@��{�A�E�C���h�E�T�C�Y�̏c����������΂悢
	// 3: near ������͈͂̈�Ԏ�O�̎��_����̋���
	// 4: far  ������͈͂̈�ԉ��̎��_����̋���
	XMMATRIX mxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

	// �`��Ŏg�p����e�N�X�`�����w�肷��֐��Ăяo��
	// �����Ŏw�肳�ꂽ�e�N�X�`�����A�s�N�Z���V�F�[�_�[�̃O���[�o���ϐ��ɃZ�b�g�����
	d3d->context->PSSetShaderResources(0, 1, &mModelData.mSRV);

	// �`��Ɏg�����_�o�b�t�@���w�肷��
	UINT strides = sizeof(VERTEX3D);
	UINT offsets = 0;
	// ��O�����F���_�o�b�t�@
	d3d->context->IASetVertexBuffers(0, 1,
		&mModelData.mVertexBuffer, &strides, &offsets);

	// �萔�o�b�t�@�X�V�@���@�b���ꂩ��V�F�[�_�[�Ƀf�[�^��]������
	D3D11_MAPPED_SUBRESOURCE pData;  // �萔�o�b�t�@�Ŋm�ۂ��ꂽ�������̈�̏�񂪊i�[�����\����
	// �萔�o�b�t�@�̃������̈�ɃA�N�Z�X���āA�f�[�^��]�����鏈��
	// �萔�o�b�t�@�̃������ɃA�N�Z�X���邽�߂ɁA���̃������̈�Ƀ��b�N��������

	HRESULT hr = d3d->context->Map(gpConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		ConstBufferData cbdata{};

		// �J�����I�u�W�F�N�g����r���[�ϊ��s����擾
		XMMATRIX mxView = mCamera->GetViewMatrix();

		cbdata.world = XMMatrixTranspose(mxWorld);
		cbdata.view = XMMatrixTranspose(mxView);
		cbdata.projection = XMMatrixTranspose(mxProjection);

		// 1: �R�s�[��������̃A�h���X
		// 2: �R�s�[��̊m�ۂ���Ă���f�[�^�T�C�Y
		// 3: �R�s�[�������f�[�^�������Ă���擪�A�h���X
		// 4: �R�s�[�������f�[�^�̃T�C�Y
		memcpy_s(pData.pData, pData.RowPitch, &cbdata, sizeof(cbdata));

		d3d->context->Unmap(gpConstBuffer, 0);  // ���b�N��������������������
	}

	// �萔�o�b�t�@���V�F�[�_�[����A�N�Z�X�ł���悤�ɃZ�b�g����
	d3d->context->VSSetConstantBuffers(1, 1, &gpConstBuffer);

	// ����܂ł̐ݒ�Ŏ��ۂɕ`�悷��  �������F���ۂɕ`�悷�钸�_��
	d3d->context->Draw(mModelData.mNumVertex, 0);
}

void Model::SetModelData(ModelData model)	{
	mModelData = model; 
}

void Model::SetScale(float newScale){
	mScale.x = newScale;
	mScale.y = newScale;
	mScale.z = newScale;
}
