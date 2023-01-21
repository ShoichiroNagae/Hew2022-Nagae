#pragma once
#include <DirectXMath.h>
//アニメーションに必要な各データを保持するヘッダー
// 使うシーンのcppにインクルード

// ****　2Dキャラ用　*****
// ビルボードオブジェクトのサイズ(w,h,uvW,uvH)
DirectX::XMFLOAT4 CHAR2DSize = DirectX::XMFLOAT4(1.0f, 1.2f, 0.33f, 0.25f);
// アニメーションの状態変数（縦軸　Ｖ座標用）
enum CHAR2DSTATE
{
	FRONT,
	LEFT,
	RIGHT,
	BACK
};
// アニメーションの基本制御   (横軸 Ｕ座標用)
// 分割数-1    より超えると描画されない
int Char2D_kihonFlame[] = { 0,1,2,1,-1 };


// 必要ごとに追加