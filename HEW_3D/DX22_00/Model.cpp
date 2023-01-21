#include "Model.h"
#include "Direct3D.h"

// �萔�o�b�t�@�p�ϐ�
ID3D11Buffer* gpConstBuffer;

using namespace DirectX; // "DirectX"namespace������ȗ�����
extern DWORD gDeltaTime; // �A�j���[�V�����Ŏg��

void Model::Draw()
{
	DIRECT3D* d3d = Direct3D_Get();

	// ��]�s��
	static float ry;

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

	ConstBufferData cbdata;
	GetWVPRMatrix(cbdata);

	cbdata.uvOffset.x = mUvOffset.x;
	cbdata.uvOffset.y = mUvOffset.y;
	cbdata.diffuse = mDiffuse;

	if (mLightActive)
	{
		cbdata.lightPower = 1.0f;
		cbdata.lightVector = XMFLOAT3(1, 1, -1);
		cbdata.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	}
	else
	{
		cbdata.lightPower = 0.0f;
	}

	HRESULT hr = d3d->context->Map(gpConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr))
	{
		// 1: �R�s�[��������̃A�h���X
		// 2: �R�s�[��̊m�ۂ���Ă���f�[�^�T�C�Y
		// 3: �R�s�[�������f�[�^�������Ă���擪�A�h���X
		// 4: �R�s�[�������f�[�^�̃T�C�Y
		const rsize_t dataSize = sizeof(cbdata);
		memcpy_s(pData.pData, pData.RowPitch, &cbdata, dataSize);

		d3d->context->Unmap(gpConstBuffer, 0);  // ���b�N��������������������
	}

	// �萔�o�b�t�@���V�F�[�_�[����A�N�Z�X�ł���悤�ɃZ�b�g����
	d3d->context->VSSetConstantBuffers(1, 1, &gpConstBuffer);// ���_�V�F�[�_�[
	d3d->context->PSSetConstantBuffers(1, 1, &gpConstBuffer);// �s�N�Z���V�F�[�_�[

	// ����܂ł̐ݒ�Ŏ��ۂɕ`�悷��  �������F���ۂɕ`�悷�钸�_��
	d3d->context->Draw(mModelData.mNumVertex, 0);
}

void Model::SetModelData(ModelData model)
{
	mModelData = model;
}

void Model::ChangeTexData(const wchar_t* pTexFileName)
{
	Direct3D_LoadTexture(pTexFileName, &mModelData.mSRV);
}

void Model::SetScale(float newScale)
{
	mScale.x = newScale;
	mScale.y = newScale;
	mScale.z = newScale;
}

void Model::Setpos(float mx, float my, float mz)
{
	this->mPos.x = mx;
	this->mPos.y = my;
	this->mPos.z = mz;
}

void Model::GetWVPRMatrix(ConstBufferData& out)
{
	XMMATRIX mxRotY = XMMatrixRotationY(
		XMConvertToRadians(mRotate.y));  // Y����]
	XMMATRIX mxRotX = XMMatrixRotationX(
		XMConvertToRadians(mRotate.x));
	XMMATRIX mxRotZ = XMMatrixRotationZ(
		XMConvertToRadians(mRotate.z));

	// ���s�ړ��s��
	XMMATRIX mxTrans = XMMatrixTranslation(
		mPos.x, mPos.y, mPos.z);  // ���̂̌��݈ʒu��\���ϐ����g���āA���s�ړ��̍s������

	// �g��k���s��
	XMMATRIX mxScale = XMMatrixScaling(
		mScale.x, mScale.y, mScale.z);

	// ���[���h�ϊ��s����쐬
	XMMATRIX mxRotate = mxRotZ * mxRotX * mxRotY;
	XMMATRIX mxWorld = mxScale * mxRotate * mxTrans;

	XMMATRIX mxView, mxProjection;

	if (m2dRender)
	{
		// 2D�`��̏ꍇ�̓r���[�ƃv���W�F�N�V�����ϊ��s���P�ʍs��ɂ���
		mxProjection = mxView = XMMatrixIdentity();
	}
	else
	{
		// �J�����I�u�W�F�N�g����r���[�ϊ��s����擾
		mxView = Camera::mMainCamera->GetViewMatrix();//mCamera->GetViewMatrix();

		// �v���W�F�N�V�����ϊ��s����쐬
		// 1: ����p
		// 2: �A�X�y�N�g��@���@��{�A�E�C���h�E�T�C�Y�̏c����������΂悢
		// 3: near ������͈͂̈�Ԏ�O�̎��_����̋���
		// 4: far  ������͈͂̈�ԉ��̎��_����̋���
		mxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	}

	// �萔�o�b�t�@�̓]���p�\���̂ɃR�s�[�@�����̍ہA�s��͓]�u�s��ɂ���
	out.world = XMMatrixTranspose(mxWorld);
	out.view = XMMatrixTranspose(mxView);
	out.projection = XMMatrixTranspose(mxProjection);
	out.worldRotate = XMMatrixTranspose(mxRotate);
}

void Model::SetUVSplit(DirectX::XMFLOAT4 mSetUV)
{
	mUVSplit = mSetUV;
}

void Model::AnimationUpdate(int setState, int* setFlame)// �A�j���[�V��������
{
	// �A�j���[�V�����̎��Ԃ��J�E���g
	mAnimTime += mAnimSpeed * gDeltaTime;

	int nowFlame = (int)mAnimTime;// ���݂̃t���[��

	if (setFlame[nowFlame] == -1)// �A�j���[�V�������Ō�܂ōs����reset
	{
		mAnimTime = 0.0f;
		nowFlame = 0;
	}

	SetUVAnimation(setState, setFlame[nowFlame]);

}

void Model::SetUVAnimation(int mState, int mNowFlame)
{
	if (mUVSplit.z == 0.00f || mUVSplit.w == 0.00f)
	{
		// UV���������Z�b�g����Ă��Ȃ��s�\���Ȏ��A�����𖳎�
	}
	else
	{
		uvWidth = mUVSplit.z;
		uvHeight = mUVSplit.w;

		float frameYoko = (float)mNowFlame; // �e�[�u�����猻�݂̃R�}���擾
		float frameTate = (float)mState; // �L�����̌������c�R�}�ԍ��Ƃ���

		float uLeft = frameYoko * uvWidth;
		float uRight = uLeft + uvWidth;
		float vTop = frameTate * uvHeight;
		float vBottom = vTop + uvHeight;

		// �ʒu��␳����
		if (fabs(uRight - 1.01f) <= 0.01f || uRight > 1.00f)//�E�[
		{
			uRight = 1.0f;
		}
		if (fabs(uLeft - 0.01f) <= 0.01f || uLeft < 0.00f)// ���[
		{
			uLeft = 0.0f;
		}
		if (fabs(vBottom - 1.01f) <= 0.01f || vBottom > 1.00f)// ���[
		{
			vBottom = 1.0f;
		}
		if (fabs(vTop - 0.01f) <= 0.01f || vTop < 0.00f)// ��[
		{
			vTop = 0.0f;
		}

		// ���f�����_�f�[�^�쐬
		const float w = mUVSplit.x / 2.0f;
		const float h = mUVSplit.y / 2.0f;

		ModelVertex vx[6];
		vx[0] = { -w,  h, 0, 1, 1, 1, 1, uLeft, vTop };      // ����
		vx[1] = { w,  h, 0, 1, 1, 1, 1, uRight, vTop };    // �E��
		vx[2] = { w, -h, 0, 1, 1, 1, 1, uRight, vBottom };// �E��

		vx[3] = { w, -h, 0, 1, 1, 1, 1, uRight, vBottom };// �E��
		vx[4] = { -w, -h, 0, 1, 1, 1, 1,uLeft, vBottom };   // ����
		vx[5] = { -w,  h, 0, 1, 1, 1, 1, uLeft, vTop };      // ����
		mModelData.mNumVertex = sizeof(vx) / sizeof(ModelVertex); // ���_�����v�Z

		// ���f���p�̒��_�o�b�t�@�쐬
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = sizeof(vx);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		// �o�b�t�@�̏����l�w��
		D3D11_SUBRESOURCE_DATA initialVertexData;
		// ���_�o�b�t�@�̏����l
		initialVertexData.pSysMem = vx;
		// ���_�o�b�t�@�ł͎g�p���Ȃ�
		initialVertexData.SysMemPitch = 0;
		initialVertexData.SysMemSlicePitch = 0;

		HRESULT hr = Direct3D_Get()->device->CreateBuffer(&bufferDesc, &initialVertexData, &mModelData.mVertexBuffer);

		if (FAILED(hr))
		{
			throw hr;
		}

	}
}