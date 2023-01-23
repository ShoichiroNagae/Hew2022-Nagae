#include "FrameControl.h"

FrameControl::FrameControl()
{
	// 取得時間の精度を１ミリ秒に変更
	timeBeginPeriod(1);

	// メンバ変数の初期化
	this->Init();
}

FrameControl::~FrameControl()
{
	// 取得時間の精度を元に戻す
	timeEndPeriod(1);
}

void FrameControl::ControlFrame(void)
{
	// 参照型変数を定義
	const DWORD frame = this->m_frameTime;
	DWORD& now = this->m_nowTime;
	DWORD& prev = this->m_prevTime;

	// 現在の時間を保存
	now = timeGetTime();

	// 処理終了後、経過時間が規定時間を下回っていたら
	if (now - prev >= frame)
	{
		// 規定時間になるまで待つ
		Sleep(1);

		// 現在の時間を更新する
		now = timeGetTime();
	}

	// 現在の時間を過去の時間として保存する
	prev = now;
}

void FrameControl::Init(void)
{
	this->m_nowTime = 0;
	this->m_prevTime = 0;
}
