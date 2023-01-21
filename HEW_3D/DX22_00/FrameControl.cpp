#include <tchar.h>
#include <sstream>
#include "FrameControl.h"

FrameControl::FrameControl()
{
	// 精度を取得
	QueryPerformanceFrequency(&m_timeFreq);

	// 初期計算のために時間を取得
	QueryPerformanceCounter(&m_timeStart);

	m_frameRate = 0.0f;
}

FrameControl::~FrameControl()
{
}

void FrameControl::ControlFrame(void)
{
	// 現在の時間を取得
	QueryPerformanceCounter(&m_timeEnd);

	// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	m_frameTime = static_cast<float>(m_timeEnd.QuadPart - m_timeStart.QuadPart) / static_cast<float>(m_timeFreq.QuadPart);

	// 時間に余裕があった場合
	if (m_frameTime < MIN_FRAME_TIME)
	{
		// ミリ秒に変換する
		DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - m_frameTime) * 1000);
		
		timeBeginPeriod(1);	// Sleepの精度を上げる
		Sleep(sleepTime);	// 待つ
		timeEndPeriod(1);	// 精度を戻す

		return;
	}

	// 経過時間が0より大きい場合
	if (m_frameTime > 0.0) { // 経過時間が0より大きい(こうしないと下の計算でゼロ除算になると思われ)
		m_frameRate = (m_frameRate * 0.99f) + (0.01f / m_frameTime); // 平均fpsを計算
#ifdef _DEBUG
// デバッグ用(デバッガにFSP出す)
#ifdef UNICODE
		std::wstringstream stream;
#else
		std::stringstream stream;
#endif
		stream << m_frameRate << " FPS" << std::endl;
		// カウンタ付けて10回に1回出力、とかにしないと見づらいかもね
		OutputDebugString(stream.str().c_str());
#endif // _DEBUG

	}

	// 入れ替え
	m_timeStart = m_timeEnd;
}

void FrameControl::Init(void)
{
}
