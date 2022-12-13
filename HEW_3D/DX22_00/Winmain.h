#pragma once

#pragma comment (lib, "winmm.lib") // timeGetTime関数のため

// マクロ定義
constexpr auto CLASS_NAME = "DX21Smpl";					// ウインドウクラスの名前;
constexpr auto WINDOW_NAME = "HEW 2022 Nagae";				// ウィンドウの名前;

constexpr auto SCREEN_WIDTH = (1024);	// ウインドウの幅;
constexpr auto SCREEN_HEIGHT = (576 + 30);	// ウインドウの高さ;

// 構造体の定義


// 関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ゲームの画面描画の関数
void Game_Init();
void Game_Draw();
void Game_Update();
void Game_Release();

void MeasureDeltaTime();

// グローバル変数の宣言

// 頂点バッファ用の変数
ID3D11Buffer* gpVertexBuffer;

// 頂点数を持つ変数
int gNumVertex;

// テクスチャ用の変数
ID3D11ShaderResourceView* gpTextureHalOsaka; // HAL OSAKA

// GameObjectクラスの変数
GameObject* gpCottage;
GameObject* gpGun;
GameObject* gpPlayer;
GameObject* gpSword;

constexpr auto MAX_GROUND = 10;
GameObject* gpGround[MAX_GROUND][MAX_GROUND];

// モデルマネージャー
// 連想配列 - 添字を整数以外にできる配列

std::map<std::string, ModelData> gModelManager;


// Cameraクラスの変数
Camera* gpCamera;

// デルタタイム用の変数
DWORD gDeltaTime;

//// テクスチャ配列(動作が重くなった為未実装)
 float animTime = 0.000f;
 int animFlame = 0;
//#define AN_DEFOMAX 3
 const wchar_t* animDefo[] =
 { 
	 L"assets/cottage.png",
	 L"assets/ground1.png",
	 L"assets/sword.png"
 };