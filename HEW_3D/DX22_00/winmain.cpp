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
#include "BillboardObject.h"

#include <map>
#include <string>
#include "Winmain.h"

extern ID3D11Buffer* gpConstBuffer;
using namespace DirectX;

// WinMain関数を作る
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wc{}; // WND = Window

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

	HWND hWnd;					// H=Handle=ポインタ WND=Window
	hWnd = CreateWindowEx(0,	// 拡張ウィンドウスタイル
		CLASS_NAME,				// ウィンドウクラスの名前
		WINDOW_NAME,			// ウィンドウの名前
		WS_OVERLAPPED
		| WS_SYSMENU
		| WS_MINIMIZEBOX,		// ウィンドウスタイル
		CW_USEDEFAULT,			// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,			// ウィンドウの左上Ｙ座標 
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
			MeasureDeltaTime();	//デルタタイム計測
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

// デルタタイムの計測
void MeasureDeltaTime()
{
	static DWORD lastTime = timeGetTime(); // 前回計測時の時間
	timeBeginPeriod(1); // 精度を１ミリ秒に上げる
	DWORD nowTime = timeGetTime(); // 現在の時間
	timeEndPeriod(1); // 精度を元に戻す
	gDeltaTime = nowTime - lastTime; // 差分がデルタタイム
	lastTime = nowTime; // 前回計測時間として保存
}

void Game_Init()
{
	// 定数バッファ作成
	// コンスタントバッファとして作成するための情報設定
	D3D11_BUFFER_DESC contstat_buffer_desc{};
	contstat_buffer_desc.ByteWidth = 4 * 4 * 4 * 4;	// バッファのサイズ（4x4行列x4個）
	contstat_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;		// 使用方法
	contstat_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// バッファの種類(コンスタントバッファ)
	contstat_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPUアクセス設定
	contstat_buffer_desc.MiscFlags = 0;				// その他のオプション
	contstat_buffer_desc.StructureByteStride = 0;			// 構造体サイズ(行列を使うが今回は0でも動作することを実証する)

	Direct3D_Get()->device->CreateBuffer(&contstat_buffer_desc,
		nullptr, &gpConstBuffer);

	// カメラの作成
	gpCamera = new BackCamera();

	// 初期値設定
	// 注意：eyeとfocusが同じだとダメ
	// 注意：upのxyz全てゼロだとダメ
	gpCamera->SetEye(XMFLOAT3(0.0f, 0.0f, -2.0f));
	gpCamera->SetFocus(XMFLOAT3(0.0f, 0.0f, 0.0f));
	gpCamera->SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));

	// ローダーに移行
	// コテージモデル読み込み
	ObjModelLoader loader;
	gModelManager["cottage"] = loader.Load(
		"assets/cottage.obj", L"assets/cottage.png");

	// コテージ用Modelオブジェクト生成
	gpCottage = new GameObject();
	Model* pCottageModel = gpCottage->GetModel();
	pCottageModel->SetModelData(gModelManager["cottage"]); // 3Dデータをセットする
	pCottageModel->SetScale(0.0005f);
	pCottageModel->mPos.z = 2.0f;
	pCottageModel->mCamera = gpCamera;

	//// 銃モデル読み込み
	//loader = ObjModelLoader(); // ローダーを再呼び出し
	//gModelManager["gun"] = loader.Load(
	//	"assets/gun.obj", L"assets/gun.png");

	//// 銃用Modelオブジェクト生成
	//gpGun = new GameObject();
	//Model* pGunModel = gpGun->GetModel();
	//pGunModel->SetModelData(gModelManager["gun"]);
	//pGunModel->SetScale(1.5f);
	//pGunModel->mPos.z = 1.0f;
	//pGunModel->mRotate.y = 90.0f;
	//pGunModel->mCamera = gpCamera;

	// 地面モデル読み込み
	loader = ObjModelLoader();
	gModelManager["ground1"] = loader.Load(
		"assets/ground1.obj", L"assets/ground1.jpg"
	);

	// 地面生成
	// 2重Forループで生成
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j] = new GameObject();
			Model* pGroundModel = gpGround[i][j]->GetModel();
			pGroundModel->SetModelData(gModelManager["ground1"]);
			pGroundModel->SetScale(1.0f);
			pGroundModel->mPos.x = -10.0f + 2.0f * i;
			pGroundModel->mPos.z = -10.0f + 2.0f * j;
			pGroundModel->mPos.y = -1.0f;
			pGroundModel->mCamera = gpCamera;
		}
	}

	// プレイヤーモデル(平面データ)読み込み
	loader = ObjModelLoader();
	gModelManager["Player"] = loader.Load(
		1.0f, 1.2f, 0.33f, 0.25f, animDefo[2]);

	// プレイヤーモデル(平面データ)生成
	gpPlayer = new BillboardObject();
	Model* pPlayerModel = gpPlayer->GetModel();
	pPlayerModel->SetModelData(gModelManager["Player"]);
	pPlayerModel->SetScale(0.3f);
	pPlayerModel->mPos.z = 1.0f;
	pPlayerModel->mPos.y = 1.0f;
	pPlayerModel->mRotate.y = 90.0f;
	pPlayerModel->mCamera = gpCamera;

	

	// ソードモデル読み込み
	loader = ObjModelLoader();
	gModelManager["Sword"] = loader.Load(
		"assets/sword.obj", L"assets/sword.png"
	);

	// ソードモデル生成
	gpSword = new GameObject();
	Model* pSwordModel = gpSword->GetModel();
	pSwordModel->SetModelData(gModelManager["Sword"]);
	pSwordModel->SetScale(0.015f);
	pSwordModel->mPos.x = pPlayerModel->mPos.x + 0.5f;
	pSwordModel->mPos.z = 1.0f;
	pSwordModel->mPos.y = 1.0f;
	pSwordModel->mRotate.y = 90.0f;
	pSwordModel->mCamera = gpCamera;

	//　カメラの追従対象を指定
	((BackCamera*)gpCamera)->SetTarget(gpPlayer);

}



void Game_Draw()
{
	// DIRECT3D構造体にアクセスする
	DIRECT3D* d3d = Direct3D_Get();

	// 画面クリア（指定色で塗りつぶし）
	// 塗りつぶし色の指定（小数：0.0f〜1.0f）
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Zバッファ、ステンシルバッファクリア
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f〜1.0f)
		0);				// ステンシルバッファを0でクリアする

	// ↓　自前の描画処理をここに書く *******

	// ゲームオブジェクトを描画
	//地面
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++) {
			gpGround[i][j]->Draw();
		}
	}
	gpCottage->Draw();
	gpPlayer->Draw();
	gpSword->Draw();

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);

}

void Game_Update()
{
	// デルタタイムが想定外の値になった場合の処理
	if (gDeltaTime >= 100) {
		gDeltaTime = 0;
	}
	if (gDeltaTime <= 0) {
		gDeltaTime = 1;
	}
	// ****** カメラ処理 *************
		// カメラ移動変数
	static float angle = 0.0f; // 回転角度
	static float zoom = 3.0f;  // ズーム

	// �@カメラの位置をキー操作で移動する
	if (Input_GetKeyDown(VK_RIGHT))
	{
		angle += 0.04f * gDeltaTime;
	}
	if (Input_GetKeyDown(VK_LEFT))
	{
		angle -= 0.04f * gDeltaTime;
	}

	// ズーム操作
	if (Input_GetKeyDown(VK_UP))
		zoom -= 0.01f * gDeltaTime;
	if (Input_GetKeyDown(VK_DOWN))
		zoom += 0.01f * gDeltaTime;
	// �Aカメラの注視点を中心にカメラを回転する

	// カメラ位置X　＝　sinf(角度ラジアン)
	// カメラ位置Z　＝　cosf(角度ラジアン)
	// 原点を中心に半径1.0fの円周上の点を求める
	float radian = XMConvertToRadians(angle);
	gpCamera->mEye.x =
		sinf(radian) * zoom;
	gpCamera->mEye.z =
		cosf(radian) * zoom + 2.0f;
	gpCamera->mEye.y = 2.0f;

	// カメラ注視点をコテージの位置にする
	gpCamera->SetFocus(gpCottage->GetModel()->mPos);

	//*******************************************
		// キャラクター移動
		// キャラクターが向いている方向に前進する
		// 向き変更＝ADキー　前進＝Wキー
		// 「前向きベクトル」を計算する
		// 移動速度＝Wキーで決まる

		//gpGun->mSpeed = 0.0f;
		//if (Input_GetKeyDown('W'))
		//	gpGun->mSpeed = 0.001f;
		//if (Input_GetKeyDown('S'))
		//	gpGun->mSpeed = -0.001f;

	gpPlayer->mSpeed = 0.0f;
	gpSword->mSpeed = 0.0f;

	if (Input_GetKeyDown('W')) {
		gpPlayer->mSpeed = 0.001f;
		gpSword->mSpeed = gpPlayer->mSpeed;
	}

	if (Input_GetKeyDown('S')) {
		gpPlayer->mSpeed = -0.001f;
		gpSword->mSpeed = gpPlayer->mSpeed;
	}


	// キャラクターの方向転換
	//Model* pGunModel = gpGun->GetModel();
	//if (Input_GetKeyDown('A'))
	//	pGunModel->mRotate.y -= 0.04f * gDeltaTime;
	//if (Input_GetKeyDown('D'))
	//	pGunModel->mRotate.y += 0.04f * gDeltaTime;

	Model* pSwordModel = gpSword->GetModel();
	Model* pPlayerModel = gpPlayer->GetModel();
	if (Input_GetKeyDown('A')) {
		pPlayerModel->mRotate.y -= 0.04f * gDeltaTime;
		pSwordModel->mRotate.y = pPlayerModel->mRotate.y;
	}
	if (Input_GetKeyDown('D')) {
		pPlayerModel->mRotate.y += 0.04f * gDeltaTime;
		pSwordModel->mRotate.y = pPlayerModel->mRotate.y;
	}
	// ********** 当たり判定 ************
		// pSwordModel to 敵ModelmScaleの関数で判定を取る
		// 
	// **********************************

	// ***** テクスチャ書き換え（仮） **************
		// 時間変更(重い)
	
	if ((int)animTime != animFlame) {
		if (animTime >= 3.0f) {
			animTime = 0.0f;
		}
		animFlame = (int)animTime;
		pPlayerModel->ChangeTexData(animDefo[(int)animFlame]);
	}
	animTime += 0.004f * gDeltaTime;
		// 配列型未実装
		//pPlayerModel->ChangeTexData((int)animDefo[animTime]);
	
	
	// 条件変更
	//if (Input_GetKeyDown(VK_SPACE))
	//	pPlayerModel->ChangeTexData(L"assets/ground1.jpg");
	
// **********************************

	gpCottage->Update();
	/*gpGun->Update();*/
	gpPlayer->Update();
	gpSword->Update();

	// カメラアップデート
	gpCamera->Update();

	for (int i = 0; i < MAX_GROUND; i++) {
		for (int j = 0; j < MAX_GROUND; j++) {
			gpGround[i][j]->Update();
		}
	}
}

void Game_Release()
{
	delete gpGun;
	delete gpCottage;
	delete gpPlayer;
	delete gpSword;

	for (int i = 0; i < MAX_GROUND; i++) {
		for (int j = 0; j < MAX_GROUND; j++) {
			delete gpGround[i][j];
		}
	}

	//モデルマネージャーが管理するモデルを全開放する
	// 先頭のイテレータを返す
	for (auto obj = gModelManager.begin();
		// 最後のイテレータと一致する？？
		obj != gModelManager.end();
		obj++)
	{
		// first → 添字
		// second → 格納されている要素

		COM_SAFE_RELEASE(obj->second.mSRV);
		COM_SAFE_RELEASE(obj->second.mVertexBuffer);
	};

	// 連想配列の要素を全削除
	gModelManager.clear();

	COM_SAFE_RELEASE(gpConstBuffer);
}
