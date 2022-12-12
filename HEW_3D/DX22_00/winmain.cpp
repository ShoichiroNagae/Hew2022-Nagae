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
#include <map>  // 連想配列
#include <vector>
#include <xstring>
#include "winmain.h"

#include "SceneManager.h"


#pragma comment (lib, "winmm.lib") // timeGetTime関数のため

// ゲーム初期化関数
void Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
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
}

// WinMain関数を作る
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
 	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ゲームの初期化
	Initialize(hInstance,nCmdShow);

	SceneManager* sceneManager = new SceneManager();
	sceneManager->ChangeScene(SceneManager::TITLE);

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
			sceneManager->Update();
			sceneManager->Draw();
			//Game_Update(); // ゲーム処理
			//Game_Draw();   // ゲーム描画
			Input_Refresh(); // キー状態の更新
		}
	}
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