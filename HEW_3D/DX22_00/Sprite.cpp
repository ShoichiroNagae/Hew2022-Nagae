#include "Sprite.h"

Sprite::Sprite()
{
	HRESULT hr;
	DIRECT3D* d3d = Direct3D_Get(); // まずDIRECT3D構造体にアクセス
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(VERTEX2D) * 6; // VRAMに確保するデータサイズ。通常は今から送る頂点データのサイズ。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 頂点バッファ作成をここで指定。
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// バッファ作成関数（デバイスクラスのメンバ関数）を呼び出し
	// キャラ01用
	hr = d3d->device->CreateBuffer(&bufferDesc, NULL,
		&mVertexBuffer); // 作った頂点バッファはここで指定した変数に保存される

	if (FAILED(hr))
	{
		// 頂点バッファの作成に失敗したときの処理をここに書く
	}

	mCenterX = 0.0f;
	mCenterY = 0.0f;

	mSizeX = 1.0f;
	mSizeY = 1.0f;

	mColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // デフォルトは白
}

void Sprite::Update()
{
	// mCenterX, mCenterYがキャラクターの中心座標
	// そこからスプライトの４頂点を計算する。
	float charWidth = mSizeX;	// スプライトの横の長さ
	float charHeight = mSizeY;	// スプライトの縦の長さ
	float xLeft = mCenterX - charWidth / 2.0f;	// スプライトの左端X
	float xRight = xLeft + charWidth;		// スプライトの右端X
	float yTop = mCenterY + charHeight / 2.0f;	// スプライトの上端Y
	float yBottom = yTop - charHeight;		// スプライトの下端Y

	VERTEX2D vx[6];
	// ポリゴンの頂点を決める
	vx[0] = { xLeft, yTop, 0.0f, 0.0f, mColor }; // { x, y, u, v } 左上
	vx[1] = { xRight, yTop, 1.0f, 0.0f, mColor }; // 右上
	vx[2] = { xRight, yBottom, 1.0f, 1.0f, mColor }; // 右下
	vx[3] = vx[2]; // 右下
	vx[4] = { xLeft, yBottom, 0.0f, 1.0f, mColor }; // 左下
	vx[5] = vx[0]; // 左上

	// DIRECT3D構造体にアクセスする
	DIRECT3D* d3d = Direct3D_Get();
	// 頂点のデータをVRAMに送る
	d3d->context->UpdateSubresource(mVertexBuffer, // 転送先の頂点バッファ
		0, NULL, vx, // 転送する配列の名前（=アドレス）
		0, 0);
}

void Sprite::Draw()
{
	// DIRECT3D構造体にアクセスする
	DIRECT3D* d3d = Direct3D_Get();

	UINT stride = sizeof(VERTEX2D);
	UINT offset = 0;

	// ↓↓↓↓　ここから背景01描画
	// ピクセルシェーダーにテクスチャを渡す
	d3d->context->PSSetShaderResources(0, 1, &mTexture);

	// ピクセルシェーダーにサンプラーを渡す
	d3d->context->PSSetSamplers(0, 1, &d3d->samplerPoint); // ポイント補完

	// 描画に使う頂点バッファの選択
	d3d->context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// 描画命令
	d3d->context->Draw(6, 0); // 第一引数＝頂点数
}

Sprite::~Sprite()
{
	// このクラスが作ったオブジェクトだけを解放する
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
