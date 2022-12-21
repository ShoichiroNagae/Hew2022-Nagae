#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"

// タイトルロゴ，スペースキーの読み込みは出来ているが，
// 表示が出来ていない

TitleScene::TitleScene()
{
	d3d = Direct3D_Get();

	// タイトルロゴを読み込む
 	LoadTexture(L"assets/2D/title.png", &gpTextureTitle);
	// 「Press Space Key」画像を読み込む
	LoadTexture(L"assets/2D/spacekey.png", &gpTextureSpaceKey);

	// タイトルロゴの初期化
	gpTitleLogo = new StaticObject();
	gpTitleLogo->mSprite->SetTexture(gpTextureTitle);
	gpTitleLogo->SetPosition(0.0f, 0.3f);
	gpTitleLogo->SetSize(10.0f, 10.0f);

	// 「Press Space Key」の初期化
	gpSpaceKey = new StaticObject();
	gpSpaceKey->mSprite->SetTexture(gpTextureSpaceKey);
	gpSpaceKey->SetPosition(0.0f, -0.3f);
		gpSpaceKey->SetSize(10.0f, 10.0f);

}

TitleScene::~TitleScene()
{
	delete gpSpaceKey;
	delete gpTitleLogo;

	COM_SAFE_RELEASE(gpTextureSpaceKey);
	COM_SAFE_RELEASE(gpTextureTitle);
}

#define StageKindNum 2  //ステージの種類数

//選択中のステージを格納する変数 0 1 2
int ActiveStageCommand = 0;

void TitleScene::Update()
{
	d3d = Direct3D_Get();

	gpTitleLogo->Update();
	gpSpaceKey->Update();

	//ゲームループ

		//タイトルシーン

		//下ボタンを押したら
	if (Input_GetKeyTrigger(VK_DOWN))
	{
		//コマンドを一つずらす（増やす）
		ActiveStageCommand++;

		//もし一番下まで行ったら
		if (ActiveStageCommand > StageKindNum)
		{
			//一番上（最初の行）に戻す
			ActiveStageCommand = 0;
		}

		//色を変更する処理
	}
	//上ボタンを押したら
	else if (Input_GetKeyTrigger(VK_UP))
	{
		//コマンドを一つずらす（減らす）
		ActiveStageCommand--;

		//もし一番上まで行ったら
		if (ActiveStageCommand < 0)
		{
			//最後尾に戻す
			ActiveStageCommand = StageKindNum;
		}

		//色を変更する処理
	}

	//もしスペースキーをしたら
	if (Input_GetKeyDown(VK_SPACE))
	{

		switch (ActiveStageCommand)
		{
		case 0:
			SceneManager::ChangeScene(SceneManager::GAME_1);
			break;
		case 1:
			SceneManager::ChangeScene(SceneManager::GAME_2);
			break;
		case 2:
			SceneManager::ChangeScene(SceneManager::GAME_3);
			break;
		default:
			break;
		}
	}
}


void TitleScene::Draw()
{
	d3d = Direct3D_Get();
	d3d->swapChain->Present(0, 0);

	// 画面の塗りつぶし処理
	//float color[4] = { 1.0f, 0.0f, 0.0f, 0.0f };  // rgba  0.0f-1.0f
	//d3d->context->ClearRenderTargetView(d3d->renderTarget,
	//	color);

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