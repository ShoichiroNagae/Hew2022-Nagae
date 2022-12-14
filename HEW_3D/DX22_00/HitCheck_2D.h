#pragma once

#include <DirectXMath.h>
using namespace DirectX;

struct Object_Info
{
	XMFLOAT3 pos;
	float size;
};

struct HITBOX
{
	float X;
	float Y;
	float Width;
	float Height;
};

class HitCheck_2D
{
public:
	// メンバ変数を初期化
	void Init(void);

	// オブジェクトの中心座標をセット
	void set_Position(XMFLOAT3 _mainPos, XMFLOAT3 _tergetPos);
	// ターゲットの座標を変更
	void set_TergetPos(XMFLOAT3 _tergetPos);

	// オブジェクトのサイズをセット
	void set_Size(float _mainSize, float _tergetSize);

	// 当たり判定を実行
	bool check_IsHit(void);

private:
	Object_Info m_MainObject;		// メインターゲットの座標
	Object_Info m_TergetObject;	// メインターゲットと当たり判定をするオブジェクトの座標

};

