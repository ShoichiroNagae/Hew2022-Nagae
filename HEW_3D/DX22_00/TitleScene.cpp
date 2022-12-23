#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"
#include "NormalObject.h"
#include "BackCamera.h"

#include <DirectXMath.h>

// �^�C�g�����S�C�X�y�[�X�L�[�̓ǂݍ��݂͏o���Ă��邪�C
// �\�����o���Ă��Ȃ�

// �R���X�g���N�^
TitleScene::TitleScene()
{
	// �J�����I�u�W�F�N�g����
	mCamera = new BackCamera();
	Camera::mMainCamera = mCamera;
	// �����l�ݒ�
	// ���ӁFeye��focus���������ƃ_��
	// ���ӁFup��xyz�S�ă[�����ƃ_��
	mCamera->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	mCamera->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	mCamera->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));

	// 2D�I�u�W�F�N�g�C���X�^���X����
	gModelManager["title"] = mPoly->CreateSquarePolygon(
		1.0f, 0.7f, 1.0f, 1.0f, L"assets/TitleLogo640.png");
	GameObject* tmp = new NormalObject();
	Model* pModel = tmp->GetModel();
	pModel->SetModelData(gModelManager["title"]);
	pModel->Set2dRender(true);	// 2D�`��ւ̐؂�ւ�
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	pModel->mCamera = mCamera;
	gObjectList.emplace_back(tmp);

	// �J�����^�[�Q�b�g��2D�I�u�W�F�N�g�Ɏw��
	dynamic_cast<BackCamera*>(mCamera)->SetTarget(gObjectList[0]);
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{

}

// �X�V����
void TitleScene::Update()
{
	// �S�C���X�^���X���X�V
	for (auto obj : gObjectList)
	{
		obj->Update();
	}

	mCamera->Update();
}

// �`�揈��
void TitleScene::Draw()
{
	// �S�C���X�^���X��`��
	for (auto obj : gObjectList)
	{
		obj->Draw();
	}
}