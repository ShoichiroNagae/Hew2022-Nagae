#pragma once

// マクロ定義
#define CLASS_NAME    "DX21Smpl"// ウインドウクラスの名前
#define WINDOW_NAME  "スケルトンプログラム"// ウィンドウの名前

#define SCREEN_WIDTH (1024)	// ウインドウの幅
#define SCREEN_HEIGHT (576+30)	// ウインドウの高さ

// 構造体の定義

// 関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ゲームの画面描画の関数
void Game_Init();
void Game_Draw();
void Game_Update();
void Game_Release();

void Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow);


// グローバル変数の宣言

// 頂点バッファ用の変数
ID3D11Buffer* gpVertexBuffer;

// 頂点数を持つ変数
int gNumVertex;

// テクスチャ用の変数
ID3D11ShaderResourceView* gpTextureHalOsaka; // HAL OSAKA

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

#define MAX_GROUND  50

// 横１０×縦１０の二次元配列
GameObject* gpGround1[MAX_GROUND];
GameObject* gpGround2[MAX_GROUND];
GameObject* gpGround3[MAX_GROUND];
GameObject* gpGround4[MAX_GROUND];

// 弾マネージャー
std::vector<GameObject*> gShotManager;

// モデルマネージャー
//// 連想配列＝添え字を整数以外にできる配列
//std::map<std::string, ModelData> gModelManager;
//
//// オブジェクトマネージャー 連想配列で作成
//std::map <std::string, GameObject*> gObjectManager;

// カメラクラスの変数
Camera* gpCamera;

// デルタタイム用の変数
DWORD gDeltaTime;