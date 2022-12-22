#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"
#include "NormalObject.h"

#include <DirectXMath.h>

// �^�C�g�����S�C�X�y�[�X�L�[�̓ǂݍ��݂͏o���Ă��邪�C
// �\�����o���Ă��Ȃ�

// �R���X�g���N�^
TitleScene::TitleScene()
{

}

// �f�X�g���N�^
TitleScene::~TitleScene()
{

}

// �X�V����
void TitleScene::Update()
{

}

// �`�揈��
void TitleScene::Draw()
{
	// 2D�I�u�W�F�N�g�̕`��
	gModelManager["title"] = mPoly->CreateSquarePolygon(
		1.0f, 0.7f, 1.0f, 1.0f, L"assets / TitleLogo640.png");
	GameObject* tmp = new NormalObject();
	Model* pModel = tmp->GetModel();
	pModel->SetModelData(gModelManager["title"]);
	pModel->Set2dRender(true);	// 2D�`��ւ̐؂�ւ�
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	gObjectList.emplace_back(tmp);
}