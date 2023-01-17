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
#include "SceneManager.h"

extern ID3D11Buffer* gpConstBuffer; //定数バッファ

// コンストラクタ
GameScene::GameScene() {
	Init();
}

// 定数バッファ作成
void GameScene::CreateConstBuffer()
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

// カメラ初期設定
void GameScene::CameraInit(Camera* cam)
{
	// カメラ初期値
	// eyeとfocusが同じ座標だとダメ
	cam->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	cam->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	// upは(0.0f,0.0f,0.0f)だとダメ
	cam->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
}

// モデル読み込み
void GameScene::ModelLoad(ObjModelLoader oml, std::string ModelName,
	const char* pObjFileName, const wchar_t* pTexFileName)
{
	oml = ObjModelLoader();
	gModelManager[ModelName] = oml.Load(
		pObjFileName, pTexFileName
	);
}

void GameScene::ModelLoad(ObjModelLoader oml,std::string ModelName,
	float width, float height, float uvWidth, float uvHeight, const wchar_t* pTexFileName)
{
	oml = ObjModelLoader();
	gModelManager[ModelName] = oml.Load(
		width, height, uvWidth, uvHeight, pTexFileName
	);
}

void GameScene::ObjectCreate(std::string objName, float mScale, float mx, float my, float mz)
{
	Model* pModel = gObjManager[objName]->GetModel();
	pModel->SetModelData(gModelManager[objName]);
	pModel->SetScale(mScale);
	pModel->Setpos(mx, my, mz);
	pModel->mCamera = gpCamera;
}

// 初期化
void GameScene::Init()
{	
	// 定数バッファ作成
	this->CreateConstBuffer();

	// カメラ作成
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;
	CameraInit(gpCamera);

	// モデル読み込み
	ModelLoad(loader, "ground1", "assets/ground1.obj", L"assets/ground1.jpg");
	ModelLoad(loader, "gun", "assets/gun.obj", L"assets/gun.png");
	ModelLoad(loader, "cottage", "assets/cottage.obj", L"assets/cottage.png");
	ModelLoad(loader, "shot", "assets/billboard.obj", L"assets/shot.png");
	ModelLoad(loader, "2Dchar", 1.0f, 1.2f, 0.33f, 0.25f, L"assets/char01.png");

	// コテージ用Modelオブジェクト生成
	gObjManager["cottage"] = new NormalObject();
	ObjectCreate("cottage", 0.001f, -10.0f, -1.0f, 0.0f);

	// 銃用Modelオブジェクト生成
	gObjManager["gun"] = new NormalObject();
	ObjectCreate("gun", 1.5f, 0.0f, 1.0f, 0.0f);

	// 2Dキャラオブジェクト生成
	gObjManager["2Dchar"] = new BillboardObject();
	ObjectCreate("2Dchar", 1.0f, -10.0f, 1.0f, 0.8f);

	// 地面を生成
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j] = new NormalObject();
			Model* pGroundModel = gpGround[i][j]->GetModel();
			pGroundModel->SetModelData(gModelManager["ground1"]);
			pGroundModel->SetScale(1.0f);
			pGroundModel->Setpos(-10.0f + 2.0f * j, -2.0f, -10.0f + 2.0f * i);
			pGroundModel->mCamera = gpCamera;
		}
	}

	// 追従カメラが追従する対象を設定
	((BackCamera*)gpCamera)->SetTarget(gObjManager["gun"]);
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
	// →　無段階で移動できる
	// →「前向きベクトル」を使う
	gObjManager["gun"]->mSpeed = 0.0f;

	// 銃の前進
	gObjManager["gun"]->mSpeed = 0.001f;

	// 主人公の移動
	Model* pGunModel = gObjManager["gun"]->GetModel();
	if (Input_GetKeyDown('W')) pGunModel->mPos.y += 0.001f;
	if (Input_GetKeyDown('S')) pGunModel->mPos.y -= 0.001f;
	if (Input_GetKeyDown('A')) pGunModel->mPos.z -= 0.001f;
	if (Input_GetKeyDown('D')) pGunModel->mPos.z += 0.001f;
	// 加速・減速
	if (Input_GetKeyDown('R')) pGunModel->mPos.x -= 0.001f;
	if (Input_GetKeyDown('F')) pGunModel->mPos.x += 0.001f;

	// 敵の自動生成(仮)
	// 
	if (Input_GetKeyDown('P')) {
		ModelLoad(loader, "2Dchar2", 1.0f, 1.2f, 0.33f, 0.25f, L"assets/char01.png");
		// 2Dキャラオブジェクト生成
		gObjManager["2Dchar2"] = new BillboardObject();
		Model*  pModel = gObjManager["2Dchar2"]->GetModel();
		pModel->SetModelData(gModelManager["2Dchar2"]);
		pModel->SetScale(1.0f);
		pModel->mPos.x = -9.0f;
		pModel->mPos.y = 1.0f;
		pModel->mPos.z = 0.8f;
		pModel->mCamera = gpCamera;
	}

	// 背景テスト
	if (Input_GetKeyDown('L')) {
		ModelLoad(loader, "BackGround", 1000.0f, 1000.0f, 1.0f, 1.0f, L"assets/ground1.jpg");
		// オブジェクト生成
		gObjManager["BackGround"] = new BillboardObject();
		Model* pModel = gObjManager["BackGround"]->GetModel();
		pModel->SetModelData(gModelManager["BackGround"]);
		pModel->SetScale(1.0f);
		pModel->mPos.x = -100.0f;
		pModel->mPos.y = 1.0f;
		pModel->mPos.z = 0.8f;
		pModel->mCamera = gpCamera;
	}

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

	// テストでリザルト画面に遷移
	if (Input_GetKeyDown('P')) {
		SceneManager::ChangeScene(SceneManager::RESULT);
	}
}

void GameScene::Draw()
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

// 解放
void GameScene::Release()
{
	// 弾管理配列の中身をすべて削除する


	// 地面の要s素をすべて削除する
	for (int i = 0; i < MAX_GROUND; i++) {
		for (int j = 0; j < MAX_GROUND; j++) {
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

// デストラクタ
GameScene::~GameScene() {
	Release();
}
