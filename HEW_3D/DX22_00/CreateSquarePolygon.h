#pragma once
#include "ModelData.h"
#include "Direct3D.h"

// ビルボード用の平面ポリゴンを作る関数
ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile)
{
	// モデル頂点データ作成
	const float w = width / 2.0f;
	const float h = height / 2.0f;
	ModelVertex vx[] = {
		// (x, y, z), (r, g, b, a), (u, v)
		// 正面
		{ -w,  h, 0, 1, 1, 1, 1, 0.0f, 0.0f },
		{  w,  h, 0, 1, 1, 1, 1, uvWidth, 0.0f },
		{  w, -h, 0, 1, 1, 1, 1, uvWidth, uvHeight },

		{  w, -h, 0, 1, 1, 1, 1, uvWidth, uvHeight },
		{ -w, -h, 0, 1, 1, 1, 1, 0.0f, uvHeight },
		{ -w,  h, 0, 1, 1, 1, 1, 0.0f, 0.0f },
	};

	ModelData modelData;

	modelData.mNumVertex = sizeof(vx) / sizeof(ModelVertex); // 頂点数を計算

	// モデル用の頂点バッファ作成
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(vx);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// バッファの初期値指定
	D3D11_SUBRESOURCE_DATA initialVertexData;
	// 頂点バッファの初期値
	initialVertexData.pSysMem = vx;
	// 頂点バッファでは使用しない
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