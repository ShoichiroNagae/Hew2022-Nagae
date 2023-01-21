#pragma once

#include <Windows.h>
#pragma comment (lib, "winmm.lib")

// 60FPS�œ������ꍇ�A�P�t���[���ɂ����鎞�Ԃ͖�P�U�~���b�����ǁA���x�̖���J��グ�̂P�V�~���ɐݒ肷��
#define DEFALT_FRAME_TIME	(1.0f / 60)	// �P�ʁ@�~���b

// �t���[�����[�g��60FPS�ŌŒ肷��N���X
class FrameControl
{
public:
	// �R���X�g���N�^
	// ���Ԏ擾�̐��x���グ�邽�߁A���C�����[���O��new���邱��
	FrameControl();

	// �f�X�g���N�^
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
	const float			MIN_FRAME_TIME = 1.0f / 60.0f;	// �t���[�����[�g�@�萔
	float				m_frameTime;
	float				m_frameRate;					// �t���[�����[�g�ۑ��p
	LARGE_INTEGER		m_timeStart;	
	LARGE_INTEGER		m_timeEnd;
	LARGE_INTEGER		m_timeFreq;

};

