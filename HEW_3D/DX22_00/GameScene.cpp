#include "GameScene.h"
#include "Direct3D.h"
#include <DirectXMath.h>
#include "ObjModelLoader.h"
#include "NormalObject.h"
#include "BillboardObject.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"

#include "CreateSquarePolygon.h"

extern ID3D11Buffer* gpConstBuffer; //定数バッファ

void GameScene::Init()
{	
	// 定数バッファ作成
	// コンスタントバッファとして作成するための情報設定
	D3D11_BUFFER_DESC contstat_buffer_desc{};
	contstat_buffer_desc.ByteWidth = 4 * 4 * 4 * 4 *4;	// バッファのサイズ（4x4行列x4個）
	contstat_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;		// 使用方法
	contstat_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// バッファの種類(コンスタントバッファ)
	contstat_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPUアクセス設定
	contstat_buffer_desc.MiscFlags = 0;				// その他のオプション
	contstat_buffer_desc.StructureByteStride = 0;			// 構造体サイズ(行列を使うが今回は0でも動作することを実証する)

	Direct3D_Get()->device->CreateBuffer(&contstat_buffer_desc,
		nullptr, &gpConstBuffer);

	// カメラ作成
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;

	// カメラ初期値
	// eyeとfocusが同じ座標だとダメ
	gpCamera->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	gpCamera->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	// upは(0.0f,0.0f,0.0f)だとダメ
	gpCamera->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));

	// コテージモデル読み込み
	ObjModelLoader loader;
	gModelManager["cottage"] = loader.Load(
		"assets/cottage.obj", L"assets/cottage.png");
	// コテージ用Modelオブジェクト生成
	gObjManager["cottage"] = new NormalObject();
	Model* pModel = gObjManager["cottage"]->GetModel();
	pModel->SetModelData(gModelManager["cottage"]); // 3Dデータをセット
	pModel->SetScale(0.001f);
	pModel->mPos.z = 4.0f;
	pModel->mPos.y = 0.0f;
	pModel->mCamera = gpCamera;

	// 銃モデル読み込み
	loader = ObjModelLoader();
	gModelManager["gun"] = loader.Load(
		"assets/gun.obj", L"assets/gun.png");
	// 銃用Modelオブジェクト生成
	gObjManager["gun"] = new NormalObject();
	pModel = gObjManager["gun"]->GetModel();
	pModel->SetModelData(gModelManager["gun"]);
	pModel->SetScale(1.5f);
	pModel->mPos.z = 0.0f;
	pModel->mPos.y = 0.8f;
	pModel->mPos.x = 0.0f;
	pModel->mRotate.y = 0.0f;
	pModel->mCamera = gpCamera;

	// 地面モデル読み込み
	loader = ObjModelLoader();
	gModelManager["ground1"] = loader.Load(
		"assets/ground1.obj", L"assets/ground1.jpg"
	);
	// コテージモデルオブジェクト生成
	gObjManager["cottage"] = new NormalObject();
	pModel = gObjManager["cottage"]->GetModel();
	pModel->SetModelData(gModelManager["cottage"]);
	pModel->SetScale(1.5f);
	pModel->mPos.z = 0.0f;
	pModel->mPos.y = 0.8f;
	pModel->mPos.z = 0.0f;
	pModel->mRotate.y = 0.0f;
	pModel->mCamera = gpCamera;
	// 弾（ビルボード）用モデル読み込み
	loader = ObjModelLoader();
	gModelManager["shot"] = loader.Load(
		"assets/billboard.obj", L"assets/shot.png"
	);

	//// 2Dキャラモデル読み込み
	loader = ObjModelLoader();
	gModelManager["2Dchar"] = loader.Load(
		1.0f, 1.2f, 0.33f, 0.25f, L"assets/char01.png");

	// 2Dキャラオブジェクト生成
	gObjManager["2Dchar"] = new BillboardObject();
	pModel = gObjManager["2Dchar"]->GetModel();
	pModel->SetModelData(gModelManager["2Dchar"]);
	pModel->SetScale(1.0f);
	pModel->mPos.x = -10.0f;
	pModel->mPos.y = 1.0f;
	pModel->mPos.z = 0.8f;
	pModel->mCamera = gpCamera;

	// 地面を生成
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j] = new NormalObject();
			Model* pGroundModel = gpGround[i][j]->GetModel();
			pGroundModel->SetModelData(gModelManager["ground1"]);
			pGroundModel->SetScale(1.0f);
			pGroundModel->mPos.x = -10.0f + 2.0f * j;
			pGroundModel->mPos.z = -10.0f + 2.0f * i;
			pGroundModel->mPos.y = -1.0f;
			pGroundModel->mCamera = gpCamera;
		}
	}

	// 追従カメラが追従する対象を設定
	((BackCamera*)gpCamera)->SetTarget(gObjManager["gun"]);
}

// 初期化
GameScene::GameScene(){
	Init();
}

// 解放
GameScene::~GameScene(){
	Release();
}

void GameScene::Update()
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
	gObjManager["gun"]->mSpeed = 0.0f;

	// 銃の前進
	gObjManager["gun"]->mSpeed = 0.001f;

	// 銃の移動
	Model* pGunModel = gObjManager["gun"]->GetModel();
	if (Input_GetKeyDown('W'))
		pGunModel->mPos.y += 0.001f;

	if (Input_GetKeyDown('S'))
		pGunModel->mPos.y -= 0.001f;

	if (Input_GetKeyDown('A'))
		pGunModel->mPos.z -= 0.001f;

	if (Input_GetKeyDown('D'))
		pGunModel->mPos.z += 0.001f;
	// 加速・減速
	if (Input_GetKeyDown('R'))
		pGunModel->mPos.x -= 0.001f;

	if (Input_GetKeyDown('F'))
		pGunModel->mPos.x += 0.001f;

// ************************************************************* 
	// アニメーション切り替わりテスト
	// gObjManagerから別のobjectに切り替える
	// 
	//// 条件変更
	//BillboardModel* p2DcharModel = gObjManager["2Dchar"]->GetModel();

	/*if (Input_GetKeyDown(VK_SPACE))
		p2DcharModel->ChangeTexData(L"assets/ground1.jpg");*/
// **************************************************************	

	// ゲームオブジェクトを描画
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Update();

	// 弾管理配列の中身をすべて更新する
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Update();

	// 地面を全て更新
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j]->Update();
		}
	}

	// カメラの更新処理（ビュー変換行列計算）
	gpCamera->Update();
}

void GameScene::Draw()
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
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j]->Draw();
		}
	}

	// ゲームオブジェクトを描画
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Draw();

	// 弾管理配列の中身をすべて描画する
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Draw();

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);
}

void GameScene::Release()
{
	// 弾管理配列の中身をすべて削除する


	// 地面の要素をすべて削除する
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			delete gpGround[i][j];
		}
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
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
	{
		delete i->second;
	}
	// 連想配列の要素を全削除
	gObjManager.clear();
}
