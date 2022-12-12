#include "Sprite.h"

Sprite::Sprite()
{
	HRESULT hr;
	DIRECT3D* d3d = Direct3D_Get(); // �܂�DIRECT3D�\���̂ɃA�N�Z�X
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(VERTEX2D) * 6; // VRAM�Ɋm�ۂ���f�[�^�T�C�Y�B�ʏ�͍����瑗�钸�_�f�[�^�̃T�C�Y�B
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���_�o�b�t�@�쐬�������Ŏw��B
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �o�b�t�@�쐬�֐��i�f�o�C�X�N���X�̃����o�֐��j���Ăяo��
	// �L����01�p
	hr = d3d->device->CreateBuffer(&bufferDesc, NULL,
		&mVertexBuffer); // ��������_�o�b�t�@�͂����Ŏw�肵���ϐ��ɕۑ������

	if (FAILED(hr))
	{
		// ���_�o�b�t�@�̍쐬�Ɏ��s�����Ƃ��̏����������ɏ���
	}

	mCenterX = 0.0f;
	mCenterY = 0.0f;

	mSizeX = 1.0f;
	mSizeY = 1.0f;

	mColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // �f�t�H���g�͔�
}

void Sprite::Update()
{
	// mCenterX, mCenterY���L�����N�^�[�̒��S���W
	// ��������X�v���C�g�̂S���_���v�Z����B
	float charWidth = mSizeX;	// �X�v���C�g�̉��̒���
	float charHeight = mSizeY;	// �X�v���C�g�̏c�̒���
	float xLeft = mCenterX - charWidth / 2.0f;	// �X�v���C�g�̍��[X
	float xRight = xLeft + charWidth;		// �X�v���C�g�̉E�[X
	float yTop = mCenterY + charHeight / 2.0f;	// �X�v���C�g�̏�[Y
	float yBottom = yTop - charHeight;		// �X�v���C�g�̉��[Y

	VERTEX2D vx[6];
	// �|���S���̒��_�����߂�
	vx[0] = { xLeft, yTop, 0.0f, 0.0f, mColor }; // { x, y, u, v } ����
	vx[1] = { xRight, yTop, 1.0f, 0.0f, mColor }; // �E��
	vx[2] = { xRight, yBottom, 1.0f, 1.0f, mColor }; // �E��
	vx[3] = vx[2]; // �E��
	vx[4] = { xLeft, yBottom, 0.0f, 1.0f, mColor }; // ����
	vx[5] = vx[0]; // ����

	// DIRECT3D�\���̂ɃA�N�Z�X����
	DIRECT3D* d3d = Direct3D_Get();
	// ���_�̃f�[�^��VRAM�ɑ���
	d3d->context->UpdateSubresource(mVertexBuffer, // �]����̒��_�o�b�t�@
		0, NULL, vx, // �]������z��̖��O�i=�A�h���X�j
		0, 0);
}

void Sprite::Draw()
{
	// DIRECT3D�\���̂ɃA�N�Z�X����
	DIRECT3D* d3d = Direct3D_Get();

	UINT stride = sizeof(VERTEX2D);
	UINT offset = 0;

	// ���������@��������w�i01�`��
	// �s�N�Z���V�F�[�_�[�Ƀe�N�X�`����n��
	d3d->context->PSSetShaderResources(0, 1, &mTexture);

	// �s�N�Z���V�F�[�_�[�ɃT���v���[��n��
	d3d->context->PSSetSamplers(0, 1, &d3d->samplerPoint); // �|�C���g�⊮

	// �`��Ɏg�����_�o�b�t�@�̑I��
	d3d->context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// �`�施��
	d3d->context->Draw(6, 0); // �����������_��
}

Sprite::~Sprite()
{
	// ���̃N���X��������I�u�W�F�N�g�������������
	COM_SAFE_RELEASE(mVertexBuffer);
}

void Sprite::SetTexture(ID3D11ShaderResourceView* pTexture)
{
	mTexture = pTexture;
}

void Sprite::SetColor(RGBA color)
{
	mColor = color;
}
