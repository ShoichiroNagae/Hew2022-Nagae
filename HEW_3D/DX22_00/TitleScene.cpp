#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"

TitleScene::TitleScene()
{
	d3d = Direct3D_Get();

	// タイトルロゴを読み込む
	LoadTexture(L"assets/title.png", &gpTextureTitle);
	// 「Press Space Key」画像を読み込む
	LoadTexture(L"assets/spacekey.png", &gpTextureSpaceKey);

	// タイトルロゴの初期化
	gpTitleLogo = new StaticObject();
	gpTitleLogo->mSprite->SetTexture(gpTextureTitle);
	gpTitleLogo->SetPosition(0.0f, 0.3f);
	gpTitleLogo->SetSize(548.0f * 0.004f, 147.0f * 0.004f);

	// 「Press Space Key」の初期化
	gpSpaceKey = new StaticObject();
	gpSpaceKey->mSprite->SetTexture(gpTextureSpaceKey);
	gpSpaceKey->SetPosition(0.0f, -0.3f);
	gpSpaceKey->SetSize(385.0f * 0.004f, 100.0f * 0.004f);

}

TitleScene::~TitleScene()
{
	delete gpSpaceKey;
	delete gpTitleLogo;

	COM_SAFE_RELEASE(gpTextureSpaceKey);
	COM_SAFE_RELEASE(gpTextureTitle);
}

void TitleScene::Update()
{
	d3d = Direct3D_Get();

	gpTitleLogo->Update();
	gpSpaceKey->Update();

	if (Input_GetKeyDown(VK_SPACE))
	{
		SceneManager::ChangeScene(SceneManager::GAME);
	}
}

void TitleScene::Draw()
{
	d3d = Direct3D_Get();

	// 画面の塗りつぶし処理
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };  // rgba  0.0f-1.0f
	d3d->context->ClearRenderTargetView(d3d->renderTarget,
		color);

	gpTitleLogo->Draw();
	gpSpaceKey->Draw();
}


void TitleScene::LoadTexture(const wchar_t* fileName, ID3D11ShaderResourceView** outTexture)
{
	HRESULT hr;
	DIRECT3D* d3d = Direct3D_Get();

	// テクスチャ読み込み
	hr = DirectX::CreateWICTextureFromFile(d3d->device,
		fileName, NULL, outTexture);

	// エラー処理
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "テクスチャ読み込み失敗", "エラー発生", MB_OK);
	}
}