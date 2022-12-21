#pragma once

#include "Direct3D.h"
#include <DirectXMath.h>

#include "GameObject.h"
#include <map>
#include <vector>

// マクロ定義
constexpr auto CLASS_NAME = "DX21Smpl";					// ウインドウクラスの名前
constexpr auto WINDOW_NAME = "スケルトンプログラム";	// ウィンドウの名前

constexpr auto SCREEN_WIDTH = (1024);					// ウインドウの幅
constexpr auto SCREEN_HEIGHT = (576 + 30);				// ウインドウの高さ

// 関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ゲーム初期化
void Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow);

// 頂点バッファ用の変数
ID3D11Buffer* gpVertexBuffer;

// 頂点数を持つ変数
int gNumVertex;

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

// デルタタイム用の変数
DWORD gDeltaTime;