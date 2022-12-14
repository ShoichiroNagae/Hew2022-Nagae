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
	// �����o�ϐ���������
	void Init(void);

	// �I�u�W�F�N�g�̒��S���W���Z�b�g
	void set_Position(XMFLOAT3 _mainPos, XMFLOAT3 _tergetPos);
	// �^�[�Q�b�g�̍��W��ύX
	void set_TergetPos(XMFLOAT3 _tergetPos);

	// �I�u�W�F�N�g�̃T�C�Y���Z�b�g
	void set_Size(float _mainSize, float _tergetSize);

	// �����蔻������s
	bool check_IsHit(void);

private:
	Object_Info m_MainObject;		// ���C���^�[�Q�b�g�̍��W
	Object_Info m_TergetObject;	// ���C���^�[�Q�b�g�Ɠ����蔻�������I�u�W�F�N�g�̍��W

};

