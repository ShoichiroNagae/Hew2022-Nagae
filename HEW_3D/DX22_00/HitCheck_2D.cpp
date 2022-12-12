#include "HitCheck_2D.h"

void HitCheck_2D::Init(void)
{
	XMFLOAT3 init;
	init.x = 0.0f;
	init.y = 0.0f;
	init.z = 0.0f;

	// 座標を初期化
	this->m_MainObject.pos = init;
	this->m_TergetObject.pos = init;
}

void HitCheck_2D::set_Position(XMFLOAT3 _mainPos, XMFLOAT3 _tergetPos)
{
	// 座標をセットする
	this->m_MainObject.pos = _mainPos;
	this->m_TergetObject.pos = _tergetPos;
}

void HitCheck_2D::set_TergetPos(XMFLOAT3 _tergetPos)
{
	// 座標をセットする
	this->m_TergetObject.pos = _tergetPos;
}

void HitCheck_2D::set_Size(float _mainSize, float _tergetSize)
{
	// サイズをセットする
	this->m_MainObject.size = _mainSize;
	this->m_TergetObject.size = _tergetSize;
}

bool HitCheck_2D::check_IsHit(void)
{
	bool IsHit = false;

	// 当たり判定の範囲をそれぞれ指定
	HITBOX mainObject;
	mainObject.X		= this->m_MainObject.pos.z;
	mainObject.Y		= this->m_MainObject.pos.y;
	mainObject.Width	= this->m_MainObject.size;
	mainObject.Height	= this->m_MainObject.size;

	HITBOX tergetObject;
	tergetObject.X		= this->m_TergetObject.pos.z;
	tergetObject.Y		= this->m_TergetObject.pos.y;
	tergetObject.Width	= this->m_TergetObject.size;
	tergetObject.Height	= this->m_TergetObject.size;

	// 判定
	//if (!((mainObject.right < tergetObject.left) || (tergetObject.right < mainObject.left) || (mainObject.bottom < tergetObject.top) || (tergetObject.bottom < mainObject.top)))
	//{
	//	IsHit = true;
	//}

	if (mainObject.X <= tergetObject.X + tergetObject.Width &&
		mainObject.X + mainObject.Width >= tergetObject.X &&
		mainObject.Y <= tergetObject.Y + tergetObject.Height &&
		mainObject.Y + mainObject.Height >= tergetObject.Y)
	{
		IsHit = true;
	}


	return IsHit;
}
