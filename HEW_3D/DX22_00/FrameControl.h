#pragma once

#include <Windows.h>
#pragma comment (lib, "winmm.lib")

// 60FPS�œ������ꍇ�A�P�t���[���ɂ����鎞�Ԃ͖�P�U�~���b�����ǁA���x�̖���J��グ�̂P�V�~���ɐݒ肷��
#define DEFALT_FRAME_TIME	(17)	// �P�ʁ@�~���b

// �t���[�����[�g��60FPS�ŌŒ肷��N���X
class FrameControl
{
public:
	FrameControl();
	~FrameControl();

	//=================================
	// ControlFrame
	//
	// ����  : �Ȃ�
	//
	// �߂�l: �Ȃ�
	//
	// �@�\  : �t���[�����[�g���Œ肷��
	// �@�@�@  �Q�[�����[�v���̍Ō���ŌĂяo��
	void ControlFrame(void);

	// �����o�ϐ��̏�����
	void Init(void);

private:
	const DWORD		m_frameTime = DEFALT_FRAME_TIME;	// 1�t���[���Ŏg�����ԁ@�i�萔�j
	DWORD			m_nowTime;							// ���݂̎��Ԃ�ۑ�����
	DWORD			m_prevTime;							// �P�t���[���O�̎��Ԃ�ۑ�����

};

