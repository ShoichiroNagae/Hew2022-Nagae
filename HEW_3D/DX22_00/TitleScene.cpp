#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"

// �^�C�g�����S�C�X�y�[�X�L�[�̓ǂݍ��݂͏o���Ă��邪�C
// �\�����o���Ă��Ȃ�

TitleScene::TitleScene()
{
	d3d = Direct3D_Get();

	// �^�C�g�����S��ǂݍ���
 	LoadTexture(L"assets/2D/title.png", &gpTextureTitle);
	// �uPress Space Key�v�摜��ǂݍ���
	LoadTexture(L"assets/2D/spacekey.png", &gpTextureSpaceKey);

	// �^�C�g�����S�̏�����
	gpTitleLogo = new StaticObject();
	gpTitleLogo->mSprite->SetTexture(gpTextureTitle);
	gpTitleLogo->SetPosition(0.0f, 0.3f);
	gpTitleLogo->SetSize(10.0f, 10.0f);

	// �uPress Space Key�v�̏�����
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
	d3d->swapChain->Present(0, 0);

	// ��ʂ̓h��Ԃ�����
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

	// �e�N�X�`���ǂݍ���
	hr = DirectX::CreateWICTextureFromFile(d3d->device,
		fileName, NULL, outTexture);

	// �G���[����
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[����", MB_OK);
	}
}