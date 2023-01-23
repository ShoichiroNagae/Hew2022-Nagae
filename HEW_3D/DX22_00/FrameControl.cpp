#include "FrameControl.h"

FrameControl::FrameControl()
{
	// �擾���Ԃ̐��x���P�~���b�ɕύX
	timeBeginPeriod(1);

	// �����o�ϐ��̏�����
	this->Init();
}

FrameControl::~FrameControl()
{
	// �擾���Ԃ̐��x�����ɖ߂�
	timeEndPeriod(1);
}

void FrameControl::ControlFrame(void)
{
	// �Q�ƌ^�ϐ����`
	const DWORD frame = this->m_frameTime;
	DWORD& now = this->m_nowTime;
	DWORD& prev = this->m_prevTime;

	// ���݂̎��Ԃ�ۑ�
	now = timeGetTime();

	// �����I����A�o�ߎ��Ԃ��K�莞�Ԃ�������Ă�����
	if (now - prev >= frame)
	{
		// �K�莞�ԂɂȂ�܂ő҂�
		Sleep(1);

		// ���݂̎��Ԃ��X�V����
		now = timeGetTime();
	}

	// ���݂̎��Ԃ��ߋ��̎��ԂƂ��ĕۑ�����
	prev = now;
}

void FrameControl::Init(void)
{
	this->m_nowTime = 0;
	this->m_prevTime = 0;
}
