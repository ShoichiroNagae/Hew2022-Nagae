#pragma once
#include "ModelData.h"
#include "Direct3D.h"

// �r���{�[�h�p�̕��ʃ|���S�������֐�
ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile)
{
	// ���f�����_�f�[�^�쐬
	const float w = width / 2.0f;
	const float h = height / 2.0f;
	ModelVertex vx[] = {
		// (x, y, z), (r, g, b, a), (u, v)
		// ����
		{ -w,  h, 0, 1, 1, 1, 1, 0.0f, 0.0f },
		{  w,  h, 0, 1, 1, 1, 1, uvWidth, 0.0f },
		{  w, -h, 0, 1, 1, 1, 1, uvWidth, uvHeight },

		{  w, -h, 0, 1, 1, 1, 1, uvWidth, uvHeight },
		{ -w, -h, 0, 1, 1, 1, 1, 0.0f, uvHeight },
		{ -w,  h, 0, 1, 1, 1, 1, 0.0f, 0.0f },
	};

	ModelData modelData;

	modelData.mNumVertex = sizeof(vx) / sizeof(ModelVertex); // ���_�����v�Z

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

	HRESULT hr = Direct3D_Get()->device->CreateBuffer(&bufferDesc, &initialVertexData, &modelData.mVertexBuffer);

	if (FAILED(hr))
	{
		throw hr;
	}

	Direct3D_LoadTexture(texFile, &modelData.mSRV);

	return modelData;
}