#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"
#include "NormalObject.h"
#include "BackCamera.h"

#include <DirectXMath.h>

// タイトルロゴ，スペースキーの読み込みは出来ているが，
// 表示が出来ていない

// コンストラクタ
TitleScene::TitleScene()
{
	// カメラオブジェクト生成
	mCamera = new BackCamera();
	Camera::mMainCamera = mCamera;
	// 初期値設定
	// 注意：eyeとfocusが同じだとダメ
	// 注意：upのxyz全てゼロだとダメ
	mCamera->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	mCamera->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	mCamera->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));

	// 2Dオブジェクトインスタンス生成
	gModelManager["title"] = mPoly->CreateSquarePolygon(
		1.0f, 0.7f, 1.0f, 1.0f, L"assets/TitleLogo640.png");
	GameObject* tmp = new NormalObject();
	Model* pModel = tmp->GetModel();
	pModel->SetModelData(gModelManager["title"]);
	pModel->Set2dRender(true);	// 2D描画への切り替え
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	pModel->mCamera = mCamera;
	gObjectList.emplace_back(tmp);

	// カメラターゲットを2Dオブジェクトに指定
	dynamic_cast<BackCamera*>(mCamera)->SetTarget(gObjectList[0]);
}

// デストラクタ
TitleScene::~TitleScene()
{

}

// 更新処理
void TitleScene::Update()
{
	// 全インスタンスを更新
	for (auto obj : gObjectList)
	{
		obj->Update();
	}

	mCamera->Update();
}

// 描画処理
void TitleScene::Draw()
{
	// 全インスタンスを描画
	for (auto obj : gObjectList)
	{
		obj->Draw();
	}
}