#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"
#include "NormalObject.h"

#include <DirectXMath.h>

// タイトルロゴ，スペースキーの読み込みは出来ているが，
// 表示が出来ていない

// コンストラクタ
TitleScene::TitleScene()
{

}

// デストラクタ
TitleScene::~TitleScene()
{

}

// 更新処理
void TitleScene::Update()
{

}

// 描画処理
void TitleScene::Draw()
{
	// 2Dオブジェクトの描画
	gModelManager["title"] = mPoly->CreateSquarePolygon(
		1.0f, 0.7f, 1.0f, 1.0f, L"assets / TitleLogo640.png");
	GameObject* tmp = new NormalObject();
	Model* pModel = tmp->GetModel();
	pModel->SetModelData(gModelManager["title"]);
	pModel->Set2dRender(true);	// 2D描画への切り替え
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	gObjectList.emplace_back(tmp);
}