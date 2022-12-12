#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う

#include <Windows.h>
#include "Direct3d.h"
#include "input.h"
#include <DirectXMath.h>
#include "WICTextureLoader.h"
#include "ObjModelLoader.h"
#include "Model.h"
#include "BackCamera.h"
#include "GameObject.h"
#include "NormalObject.h"
#include "BillboardObject.h"
#include "CreateSquarePolygon.h"
#include "HitCheck_2D.h"
#include <map>  // 連想配列
#include <vector>
#include <xstring>


#pragma comment (lib, "winmm.lib") // timeGetTime関数のため

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
// 連想配列＝添え字を整数以外にできる配列
std::map<std::string, ModelData> gModelManager;

// オブジェクトマネージャー 連想配列で作成
std::map <std::string, GameObject*> gObjectManager;

// カメラクラスの変数
Camera* gpCamera;

// デルタタイム用の変数
DWORD gDeltaTime;

// 当たり判定用の変数
HitCheck_2D* gpHit;

// WinMain関数を作る
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc; // WND = Window

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc); // Windowsにウインドウ情報を登録

	HWND hWnd; // H=Handle=ポインタ WND=Window
	hWnd = CreateWindowEx(0,// 拡張ウィンドウスタイル
		CLASS_NAME,// ウィンドウクラスの名前
		WINDOW_NAME,// ウィンドウの名前
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,// ウィンドウスタイル
		CW_USEDEFAULT,// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,// ウィンドウの左上Ｙ座標 
		SCREEN_WIDTH,// ウィンドウの幅
		SCREEN_HEIGHT,// ウィンドウの高さ
		NULL,// 親ウィンドウのハンドル
		NULL,// メニューハンドルまたは子ウィンドウID
		hInstance,// インスタンスハンドル
		NULL);// ウィンドウ作成データ

	// 指定されたウィンドウの表示状態を設定(ウィンドウを表示)
	ShowWindow(hWnd, nCmdShow);
	// ウィンドウの状態を直ちに反映(ウィンドウのクライアント領域を更新)
	UpdateWindow(hWnd);

	// Direct3Dの初期化関数を呼び出す
	Direct3D_Init(hWnd);

	Game_Init();

	

	MSG msg;
	// メインループ
	for (;;) {
		// 前回のループからユーザー操作があったか調べる
		BOOL doesMessageExist = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		if (doesMessageExist)
		{
			// 間接的にウインドウプロシージャを呼び出す
			DispatchMessage(&msg);

			// アプリ終了命令が来た
			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			// デルタタイムの計測
			static DWORD lastTime = timeGetTime(); // 前回計測時の時間
			timeBeginPeriod(1); // 精度を１ミリ秒に上げる
			DWORD nowTime = timeGetTime(); // 現在の時間
			timeEndPeriod(1); // 精度を元に戻す
			gDeltaTime = nowTime - lastTime; // 差分がデルタタイム
			lastTime = nowTime; // 前回計測時間として保存

			// ゲームループ
			Game_Update(); // ゲーム処理
			Game_Draw();   // ゲーム描画
			Input_Refresh(); // キー状態の更新
		}
	}

	Game_Release();
	// Direct3Dの解放関数を呼び出す
	Direct3D_Release();

	UnregisterClass(CLASS_NAME, hInstance);

	return (int)msg.wParam;
}


// ウインドウプロシージャ関数を作る
// ※関数を作れるのはグローバル領域(=どの関数の中でもない場所)だけ！
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// uMsg（この関数の第2引数）が、ユーザー操作のID情報
	switch (uMsg)
	{
	case WM_DESTROY:// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);// “WM_QUIT”メッセージを送る　→　アプリ終了
		break;

	case WM_CLOSE:  // xボタンが押されたら
		DestroyWindow(hWnd);  // “WM_DESTROY”メッセージを送る
		break;

	case WM_LBUTTONDOWN: // 左クリックされたとき
		break;

	case WM_RBUTTONDOWN: // 右クリックされたとき
		break;

	case WM_MOUSEMOVE: // マウスカーソルが動いたとき
		break;

	case WM_KEYDOWN:
		// キーが押された時のリアクションを書く
		// ESCが押されたのかどうかチェック
		if (LOWORD(wParam) == VK_ESCAPE)
		{
			// メッセージボックスで修了確認
			int result;
			result = MessageBox(NULL, "終了してよろしいですか？",
				"終了確認", MB_YESNO | MB_ICONQUESTION);
			if (result == IDYES) // 「はい」ボタンが押された時
			{
				// xボタンが押されたのと同じ効果を発揮する
				PostMessage(hWnd, WM_CLOSE, wParam, lParam);
			}
		}
		Input_SetKeyDown(LOWORD(wParam));
		break;

	case WM_KEYUP: // キーが離されたイベント
		Input_SetKeyUp(LOWORD(wParam));
		break;

	default:
		// 上のcase以外の場合の処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

void Game_Init()
{
	// 定数バッファ作成
	// コンスタントバッファとして作成するための情報設定
	D3D11_BUFFER_DESC contstat_buffer_desc;
	contstat_buffer_desc.ByteWidth = 4 * 4 * 4 * 4;	// バッファのサイズ（4x4行列x4個）
	contstat_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;		// 使用方法
	contstat_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// バッファの種類(コンスタントバッファ)
	contstat_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPUアクセス設定
	contstat_buffer_desc.MiscFlags = 0;				// その他のオプション
	contstat_buffer_desc.StructureByteStride = 0;			// 構造体サイズ(行列を使うが今回は0でも動作することを実証する)

	Direct3D_Get()->device->CreateBuffer(&contstat_buffer_desc,
		nullptr, &gpConstBuffer);

	// カメラ作成
	gpCamera = new BackCamera();
	// カメラ初期値
	// eyeとfocusが同じ座標だとダメ
	gpCamera->SetEye(XMFLOAT3(0.0f, 0.0f, -2.0f));
	gpCamera->SetFocus(XMFLOAT3(0.0f, 0.0f, 0.0f));
	// upは(0.0f,0.0f,0.0f)だとダメ
	gpCamera->SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));

	// コテージモデル読み込み
	ObjModelLoader loader;
	gModelManager["cottage"] = loader.Load(
		"assets/cottage.obj",
		L"assets/cottage.png");

	// 銃モデル読み込み
	ObjModelLoader loader2;
	gModelManager["gun"] = loader2.Load(
		"assets/gun.obj", L"assets/gun.png");

	// 地面モデル読み込み
	ObjModelLoader loader3;
	gModelManager["ground1"] = loader3.Load(
		"assets/ground1.obj", L"assets/ground1.jpg"
	);

	// 弾（ビルボード）用モデル読み込み
	ObjModelLoader loader4;
	gModelManager["shot"] = loader4.Load(
		"assets/billboard.obj", L"assets/shot.png"
	);

	// 2Dキャラモデル作成
	gModelManager["2Dchar"] =
	CreateSquarePolygon(1.0f, 1.2f, 0.33f, 0.25f, L"assets/char01.png");

	// gObjectManager
	// コテージ用Modelオブジェクト生成
	//gObjectManager["cottage"] = new NormalObject();
	//Model* pModel = gObjectManager["cottage"]->GetModel();
	//pModel->SetModelData(gModelManager["cottage"]); // 3Dデータをセット
	//pModel->SetScale(0.001f);
	//pModel->mPos.z = 4.0f;
	//pModel->mPos.y = 0.0f;
	//pModel->mCamera = gpCamera;

	// 銃用Modelオブジェクト生成
	gObjectManager["gun"] = new NormalObject();
	Model* pModel = gObjectManager["gun"]->GetModel();
	pModel->SetModelData(gModelManager["gun"]);
	pModel->SetScale(1.5f);
	pModel->mPos.z = 0.0f;
	pModel->mPos.y = 0.3f;
	pModel->mPos.x = 0.0f;
	pModel->mRotate.y = 0.0f;
	pModel->mCamera = gpCamera;

	// 2Dキャラオブジェクト生成
	gObjectManager["2Dchar"] = new BillboardObject();
	pModel = gObjectManager["2Dchar"]->GetModel();
	pModel->SetModelData(gModelManager["2Dchar"]);
	pModel->SetScale(1.0f);
	pModel->mPos.x = -10.0f;
	pModel->mPos.y = 1.0f;
	pModel->mPos.z = 0.8f;
	pModel->mCamera = gpCamera;


	// 地面を生成
	for (int i = 0; i < MAX_GROUND; i++)
	{
		gpGround1[i] = new NormalObject();
		Model* pGroundModel = gpGround1[i]->GetModel();
		pGroundModel->SetModelData(gModelManager["ground1"]);
		pGroundModel->SetScale(1.0f);
		pGroundModel->mPos.x = 0.0f - 2.0f * i;
		pGroundModel->mPos.z = 0.0f;
		pGroundModel->mPos.y = -1.0f;
		pGroundModel->mCamera = gpCamera;
	}

	// 追従カメラが追従する対象を設定
	((BackCamera*)gpCamera)->SetTarget(gObjectManager["gun"]);

	// 当たり判定
	gpHit = new HitCheck_2D();
	gpHit->Init();
}



void Game_Draw()
{
	// DIRECT3D構造体にアクセスする
	DIRECT3D* d3d = Direct3D_Get();

	// 画面クリア（指定色で塗りつぶし）
	// 塗りつぶし色の指定（小数：0.0f～1.0f）
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Zバッファ、ステンシルバッファクリア
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);				// ステンシルバッファを0でクリアする

	// ↓　自前の描画処理をここに書く *******
	for (int i = 0; i < MAX_GROUND; i++)
	{
		gpGround1[i]->Draw();
	}

	// ゲームオブジェクトを描画
	for (auto i = gObjectManager.begin();
		i != gObjectManager.end();
		i++)
		i->second->Draw();

	// 弾管理配列の中身をすべて描画する
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Draw();

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);
}

void Game_Update()
{
	// デルタタイムが想定外の値になった場合の処理
	if (gDeltaTime >= 100)
	{
		gDeltaTime = 0;
	}
	if (gDeltaTime <= 0)
	{
		gDeltaTime = 1;
	}

	// キャラクター移動
	// →　キャラクターが向いている方向に進ませるには？
	// 　→　無段階で移動できる
	// 　→　「前向きベクトル」を使う
	gObjectManager["gun"]->mSpeed = 0.0f;

	// 銃の前進
		gObjectManager["gun"]->mSpeed = 0.001f;

	// 銃の移動
		Model* pModel = gObjectManager["gun"]->GetModel();
		if (Input_GetKeyDown('W'))
			pModel->mPos.y += 0.001f;

		if (Input_GetKeyDown('S'))
			pModel->mPos.y -= 0.001f;

		if (Input_GetKeyDown('A'))
			pModel->mPos.z -= 0.001f;

		if (Input_GetKeyDown('D'))
			pModel->mPos.z += 0.001f;

	// ゲームオブジェクトを描画
	for (auto i = gObjectManager.begin();
		i != gObjectManager.end();
		i++)
		i->second->Update();

	// 弾管理配列の中身をすべて更新する
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Update();

	gpHit->set_Position(gObjectManager["gun"]->GetModel()->mPos, gObjectManager["2Dchar"]->GetModel()->mPos);
	gpHit->set_Size(gObjectManager["gun"]->GetModel()->mScale.x, gObjectManager["2Dchar"]->GetModel()->mScale.x);

	if (gpHit->check_IsHit())
	{
		gObjectManager.erase("2Dchar");
	}


	// カメラの更新処理（ビュー変換行列計算）
	gpCamera->Update();

	for (int i = 0; i < MAX_GROUND; i++)
	{
		gpGround1[i]->Update();
	}
}

void Game_Release()
{
	// 弾管理配列の中身をすべて削除する
	

	// 地面の要素をすべて削除する
	for (int i = 0; i < MAX_GROUND; i++)
	{
		gpGround1[i];
	}

	COM_SAFE_RELEASE(gpConstBuffer);

	// モデルマネージャーが管理するモデルを全解放する
	for (auto i = gModelManager.begin();// 一番前の要素の管理情報を返す
		i != gModelManager.end();// 一番最後の管理情報を返す
		i++)
	{
		// first … 添え字
		// second … 格納されている要素そのもの
		COM_SAFE_RELEASE(i->second.mSRV);
		COM_SAFE_RELEASE(i->second.mVertexBuffer);
	}
	// 連想配列の要素を全削除
	gModelManager.clear();

	// オブジェクトマネージャーが管理するオブジェクトを全開放する
	for (auto i = gObjectManager.begin();
		i != gObjectManager.end();
		i++)
	{
		delete i->second;
	}
	// 連想配列の要素を全削除
	gObjectManager.clear();
}