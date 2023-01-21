#pragma once

#include <Windows.h>
#pragma comment (lib, "winmm.lib")

// 60FPSで動かす場合、１フレームにかける時間は約１６ミリ秒だけど、精度の問題上繰り上げの１７ミリに設定する
#define DEFALT_FRAME_TIME	(1.0f / 60)	// 単位　ミリ秒

// フレームレートを60FPSで固定するクラス
class FrameControl
{
public:
	// コンストラクタ
	// 時間取得の精度を上げるため、メインルーム前でnewすること
	FrameControl();

	// デストラクタ
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
	const float			MIN_FRAME_TIME = 1.0f / 60.0f;	// フレームレート　定数
	float				m_frameTime;
	float				m_frameRate;					// フレームレート保存用
	LARGE_INTEGER		m_timeStart;	
	LARGE_INTEGER		m_timeEnd;
	LARGE_INTEGER		m_timeFreq;

};

