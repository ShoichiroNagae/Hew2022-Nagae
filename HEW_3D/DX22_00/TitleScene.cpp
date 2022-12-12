#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"

TitleScene::TitleScene()
{
	d3d = Direct3D_Get();

	// �^�C�g�����S��ǂݍ���
	LoadTexture(L"assets/title.png", &gpTextureTitle);
	// �uPress Space Key�v�摜��ǂݍ���
	LoadTexture(L"assets/spacekey.png", &gpTextureSpaceKey);

	// �^�C�g�����S�̏�����
	gpTitleLogo = new StaticObject();
	gpTitleLogo->mSprite->SetTexture(gpTextureTitle);
	gpTitleLogo->SetPosition(0.0f, 0.3f);
	gpTitleLogo->SetSize(548.0f * 0.004f, 147.0f * 0.004f);

	// �uPress Space Key�v�̏�����
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

	// ��ʂ̓h��Ԃ�����
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

	// �e�N�X�`���ǂݍ���
	hr = DirectX::CreateWICTextureFromFile(d3d->device,
		fileName, NULL, outTexture);

	// �G���[����
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[����", MB_OK);
	}
}