#pragma once
#include<DirectXMath.h>

// 3Dの当たり判定に必要な情報をまとめた構造体
struct OBB
{
	DirectX::XMFLOAT3 c;		// 中心座標
	DirectX::XMFLOAT3 u[3];		// 各方向のベクトル
	DirectX::XMFLOAT3 e;		// 中心点から面までの長さ
};

// 3Dデータを使った当たり判定
class OBB_HitCheck
{
public:

private:

};

