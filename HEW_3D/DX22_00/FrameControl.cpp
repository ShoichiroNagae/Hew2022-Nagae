#include <tchar.h>
#include <sstream>
#include "FrameControl.h"

FrameControl::FrameControl()
{
	// ���x���擾
	QueryPerformanceFrequency(&m_timeFreq);

	// �����v�Z�̂��߂Ɏ��Ԃ��擾
	QueryPerformanceCounter(&m_timeStart);

	m_frameRate = 0.0f;
}

FrameControl::~FrameControl()
{
}

void FrameControl::ControlFrame(void)
{
	// ���݂̎��Ԃ��擾
	QueryPerformanceCounter(&m_timeEnd);

	// (���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��)
	m_frameTime = static_cast<float>(m_timeEnd.QuadPart - m_timeStart.QuadPart) / static_cast<float>(m_timeFreq.QuadPart);

	// ���Ԃɗ]�T���������ꍇ
	if (m_frameTime < MIN_FRAME_TIME)
	{
		// �~���b�ɕϊ�����
		DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - m_frameTime) * 1000);
		
		timeBeginPeriod(1);	// Sleep�̐��x���グ��
		Sleep(sleepTime);	// �҂�
		timeEndPeriod(1);	// ���x��߂�

		return;
	}

	// �o�ߎ��Ԃ�0���傫���ꍇ
	if (m_frameTime > 0.0) { // �o�ߎ��Ԃ�0���傫��(�������Ȃ��Ɖ��̌v�Z�Ń[�����Z�ɂȂ�Ǝv���)
		m_frameRate = (m_frameRate * 0.99f) + (0.01f / m_frameTime); // ����fps���v�Z
#ifdef _DEBUG
// �f�o�b�O�p(�f�o�b�K��FSP�o��)
#ifdef UNICODE
		std::wstringstream stream;
#else
		std::stringstream stream;
#endif
		stream << m_frameRate << " FPS" << std::endl;
		// �J�E���^�t����10���1��o�́A�Ƃ��ɂ��Ȃ��ƌ��Â炢������
		OutputDebugString(stream.str().c_str());
#endif // _DEBUG

	}

	// ����ւ�
	m_timeStart = m_timeEnd;
}

void FrameControl::Init(void)
{
}
