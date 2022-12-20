#pragma once

#include "Direct3d.h"

class Sprite
{
public:
	Sprite(); 
	virtual void Update(); // 毎ループ頂点の計算行う
	void Draw(); // 描画の設定と描画
	~Sprite();  // デストラクタ＝解放処理行う

	// 読み込み済みのテクスチャを受け取る
	void SetTexture(ID3D11ShaderResourceView* pTexture);

	ID3D11ShaderResourceView* mTexture; // このスプライトで使うテクスチャ
	ID3D11Buffer* mVertexBuffer; // このスプライトで使う頂点バッファ

	float mCenterX, mCenterY; // このスプライトの中心座標
	float mSizeX, mSizeY;      // このスプライトのサイズ

	// 内部クラス
	// 色を表す構造体
	struct RGBA
	{
		float r, g, b, a;
	};

	// ポリゴンの頂点用の構造体
	struct VERTEX2D  // VERTEX = 頂点
	{
		float x, y;  // 頂点の座標（位置）
		float u, v;  // テクスチャのUV座標
		RGBA color;  // 頂点の色
	};

	// 頂点の色を持っておく変数
	RGBA mColor;

	// 頂点の色を設定する関数
	void SetColor(RGBA color);

};

