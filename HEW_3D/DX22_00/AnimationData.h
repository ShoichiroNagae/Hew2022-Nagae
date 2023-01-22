#pragma once
#include <DirectXMath.h>
//アニメーションに必要な各データを保持するヘッダー
// 使うシーンのcppにインクルード

// ****　Playerキャラ用　*****
// 
// ビルボードオブジェクトのサイズ(w,h,uW,vH)
// u座標・ｖ座標は、 『　1.00f / 分割数　』の値を書き込む
DirectX::XMFLOAT4 Player2DSize = DirectX::XMFLOAT4(1.0f, 1.2f, 0.33f, 0.25f);

// アニメーションの状態変数（縦軸　Ｖ座標用）
// 分割数-1 より超えると描画されない
enum PLAYER2DSTATE
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