#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"
#include "NormalObject.h"
#include "BackCamera.h"
#include "SceneManager.h"

#include <DirectXMath.h>

extern ID3D11Buffer* gpConstBuffer; //定数バッファ
extern bool GameFlag;

// コンストラクタ
TitleScene::TitleScene()
{
	this->Init();
}

// デストラクタ
TitleScene::~TitleScene()
{
	this->Release();
}

// 更新処理
void TitleScene::Update()
{
	// ボタンの処理
	if (Input_GetKeyDown(VK_UP) || Input_GetKeyTrigger('W'))
		selectNum++;

	if (Input_GetKeyDown(VK_DOWN) || Input_GetKeyTrigger('S'))
		selectNum--;
	
	// 上限チェック
	if (selectNum > BUTTON_AMOUNT)	{
		selectNum = 1;
	}
	else if (selectNum < 1)	{
		selectNum = BUTTON_AMOUNT;
	}

	/*================*/
	/*オブジェクト更新処理*/
	/*================*/

	// カメラ更新
	gpCamera->Update();

	// ゲームオブジェクトを更新
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Update();

	// シーン遷移
	if (Input_GetKeyTrigger(VK_SPACE))
	{
		if (selectNum == 1) {
			SceneManager::ChangeScene(SceneManager::GAME);
		}
		else if (selectNum == 2) {
			GameFlag = true;
		}

	}
}

// 描画処理
void TitleScene::Draw()
{
	// DIRECT3D構造体にアクセスする
	DIRECT3D* d3d = Direct3D_Get();

	// 画面クリア（指定色で塗りつぶし）
	// 塗りつぶし色の指定（小数：0.0f～1.0f）
	float clearColor[4] = { 0.2f, 8.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Zバッファ、ステンシルバッファクリア
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);				// ステンシルバッファを0でクリアする


	/*==========================*/
	/*===描画処理はここより下に書く==*/
	/*==========================*/
	Model* pModel = nullptr;

	// スタートボタンが選択されているとき
	if (selectNum == 1)
	{
		// 選択されているときは透明度を下げる(透明ではなくす)
		gObjManager["startButton"]->Draw();
	}
	// 終了ボタンが選択されているとき
	else if (selectNum == 2)
	{
		// 選択されているときは透明度を下げる(透明ではなくす)
		gObjManager["quitButton"]->Draw();
	}

	gObjManager["title"]->Draw();

	//// ゲームオブジェクトを更新
	//for (auto i = gObjManager.begin();
	//	i != gObjManager.end();
	//	i++)
	//	i->second->Draw();

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);
}

// 初期化
void TitleScene::Init()
{
	// 定数バッファ作成
	this->CreateConstBuffer();

	// カメラ作成
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;
	InitCamera(gpCamera);

	// モデル読み込み

	// タイトル背景読み込み
	// モデルマネージャーで管理
	gModelManager["title"] = mPoly->CreateSquarePolygon(
		2.0f, 2.0f, 1.0f, 1.0f, L"assets/Title/Title_test.png");

	// スタートボタン読み込み
	gModelManager["startButton"] = mPoly->CreateSquarePolygon(
		0.5f, 0.5f, 1.0f, 1.0f, L"assets/Title/title_Button_start.png");

	// 終了ボタン読み込み
	gModelManager["quitButton"] = mPoly->CreateSquarePolygon(
		0.5f, 0.5f, 1.0f, 1.0f, L"assets/Title/title_Button_quit.png");

	// オブジェクト生成

	// タイトル画面用オブジェクト生成
	gObjManager["title"] = new NormalObject();
	Model* pModel = gObjManager["title"]->GetModel();
	pModel->SetModelData(gModelManager["title"]);
	pModel->Set2dRender(true);	// 2D描画への切り替え
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));

	// スタートボタン用オブジェクト生成
	gObjManager["startButton"] = new NormalObject();
	pModel = gObjManager["startButton"]->GetModel();
	pModel->SetModelData(gModelManager["startButton"]);
	pModel->Set2dRender(true);	// 2D描画への切り替え
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	pModel->mPos.x = 0.6f;
	pModel->mPos.y = 0.0f;
	// 画像透明度を上げる(半透明の状態にする)

	// 終了ボタン用オブジェクト生成
	gObjManager["quitButton"] = new NormalObject();
	pModel = gObjManager["quitButton"]->GetModel();
	pModel->SetModelData(gModelManager["quitButton"]);
	pModel->Set2dRender(true);	// 2D描画への切り替え
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	pModel->mPos.x = 0.6f;
	pModel->mPos.y = -0.7f;
	// 画像透明度を上げる(半透明の状態にする)

	// 選択されているボタンを指定
	selectNum = 1;

	// 追従カメラが追従する対象を設定
	((BackCamera*)gpCamera)->SetTarget(gObjManager["title"]);
}

void TitleScene::InitCamera(Camera* cam)
{
	// カメラ初期値
// eyeとfocusが同じ座標だとダメ
	cam->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	cam->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	// upは(0.0f,0.0f,0.0f)だとダメ
	cam->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
}

void TitleScene::CreateConstBuffer()
{
	// 定数バッファ作成
// コンスタントバッファとして作成するための情報設定
	D3D11_BUFFER_DESC const_buffef_D{};
	const_buffef_D.ByteWidth = 4 * 4 * 4 * 4 * 2;				// バッファのサイズ（4x4行列x4個）
	const_buffef_D.Usage = D3D11_USAGE_DYNAMIC;					// 使用方法
	const_buffef_D.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// バッファの種類(コンスタントバッファ)
	const_buffef_D.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// CPUアクセス設定
	const_buffef_D.MiscFlags = 0;								// その他のオプション
	const_buffef_D.StructureByteStride = 0;						// 構造体サイズ(行列を使うが今回は0でも動作することを実証する)

	Direct3D_Get()->device->CreateBuffer(&const_buffef_D,
		nullptr, &gpConstBuffer);
}

void TitleScene::Release()
{
	// ゲームオブジェクトを削除
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		delete i->second;

	// モデルマネージャーが持つ全モデルを解放
	for (auto i = gModelManager.begin();//連想配列の先頭情報
		i != gModelManager.end();//連想配列の最後尾情報
		i++)
	{
		// first … 添え字
		// second … 要素そのもの
		COM_SAFE_RELEASE(i->second.mSRV);//テクスチャ
		COM_SAFE_RELEASE(i->second.mVertexBuffer);//頂点バッファ
	}
	// 連想配列の全要素を削除する（要素数０にする）
	gModelManager.clear();
}
