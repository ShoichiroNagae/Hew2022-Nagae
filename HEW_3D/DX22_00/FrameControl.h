#pragma once

#include <Windows.h>
#pragma comment (lib, "winmm.lib")

// 60FPSで動かす場合、１フレームにかける時間は約１６ミリ秒だけど、精度の問題上繰り上げの１７ミリに設定する
#define DEFALT_FRAME_TIME	(17)	// 単位　ミリ秒

// フレームレートを60FPSで固定するクラス
class FrameControl
{
public:
	FrameControl();
	~FrameControl();

	//=================================
	// ControlFrame
	//
	// 引数  : なし
	//
	// 戻り値: なし
	//
	// 機能  : フレームレートを固定する
	// 　　　  ゲームループ内の最後尾で呼び出す
	void ControlFrame(void);

	// メンバ変数の初期化
	void Init(void);

private:
	const DWORD		m_frameTime = DEFALT_FRAME_TIME;	// 1フレームで使う時間　（定数）
	DWORD			m_nowTime;							// 現在の時間を保存する
	DWORD			m_prevTime;							// １フレーム前の時間を保存する

};

