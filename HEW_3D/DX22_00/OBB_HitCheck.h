#pragma once
#include<DirectXMath.h>

// 3D�̓����蔻��ɕK�v�ȏ����܂Ƃ߂��\����
struct OBB
{
	DirectX::XMFLOAT3 c;		// ���S���W
	DirectX::XMFLOAT3 u[3];		// �e�����̃x�N�g��
	DirectX::XMFLOAT3 e;		// ���S�_����ʂ܂ł̒���
};

// 3D�f�[�^���g���������蔻��
class OBB_HitCheck
{
public:

private:

};

