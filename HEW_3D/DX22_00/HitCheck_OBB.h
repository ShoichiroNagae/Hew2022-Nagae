#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct OBB
{
	XMFLOAT3 c;		// オブジェクトの中心座標
	XMFLOAT3 u[3];	// XYZの各座標軸の傾きを表す方向ベクトル
	XMFLOAT3 e;		// OBBの各座標軸に沿った長さの半分（中心点から面までの長さ）
};

struct VERTEX
{
	XMFLOAT3 positon, normal;
	float tu, tv;
};

class HitCheck_OBB
{
public:
	void CreateOBB(OBB* obb);

private:

};

