#include "TitleScene.h"

#include "input.h"
#include "SceneManager.h"
#include "WICTextureLoader.h"
#include "NormalObject.h"
#include "BackCamera.h"
#include "SceneManager.h"

#include <DirectXMath.h>

extern ID3D11Buffer* gpConstBuffer; //�萔�o�b�t�@
extern bool GameFlag;

// �R���X�g���N�^
TitleScene::TitleScene()
{
	this->Init();
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
	this->Release();
}

// �X�V����
void TitleScene::Update()
{
	// �{�^���̏���
	if (Input_GetKeyDown(VK_UP) || Input_GetKeyTrigger('W'))
		selectNum++;

	if (Input_GetKeyDown(VK_DOWN) || Input_GetKeyTrigger('S'))
		selectNum--;
	
	// ����`�F�b�N
	if (selectNum > BUTTON_AMOUNT)	{
		selectNum = 1;
	}
	else if (selectNum < 1)	{
		selectNum = BUTTON_AMOUNT;
	}

	/*================*/
	/*�I�u�W�F�N�g�X�V����*/
	/*================*/

	// �J�����X�V
	gpCamera->Update();

	// �Q�[���I�u�W�F�N�g���X�V
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Update();

	// �V�[���J��
	if (Input_GetKeyTrigger(VK_SPACE))
	{
		if (selectNum == 1) {
			SceneManager::ChangeScene(SceneManager::GAME);
		}
		else if (selectNum == 2) {
			GameFlag = true;
		}

	}
}

// �`�揈��
void TitleScene::Draw()
{
	// DIRECT3D�\���̂ɃA�N�Z�X����
	DIRECT3D* d3d = Direct3D_Get();

	// ��ʃN���A�i�w��F�œh��Ԃ��j
	// �h��Ԃ��F�̎w��i�����F0.0f�`1.0f�j
	float clearColor[4] = { 0.2f, 8.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Z�o�b�t�@�A�X�e���V���o�b�t�@�N���A
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// �f�v�X�X�e���V���r���[
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// �y�o�b�t�@��1.0�ŃN���A����(0.0f�`1.0f)
		0);				// �X�e���V���o�b�t�@��0�ŃN���A����


	/*==========================*/
	/*===�`�揈���͂�����艺�ɏ���==*/
	/*==========================*/
	Model* pModel = nullptr;

	// �X�^�[�g�{�^�����I������Ă���Ƃ�
	if (selectNum == 1)
	{
		// �I������Ă���Ƃ��͓����x��������(�����ł͂Ȃ���)
		gObjManager["startButton"]->Draw();
	}
	// �I���{�^�����I������Ă���Ƃ�
	else if (selectNum == 2)
	{
		// �I������Ă���Ƃ��͓����x��������(�����ł͂Ȃ���)
		gObjManager["quitButton"]->Draw();
	}

	gObjManager["title"]->Draw();

	//// �Q�[���I�u�W�F�N�g���X�V
	//for (auto i = gObjManager.begin();
	//	i != gObjManager.end();
	//	i++)
	//	i->second->Draw();

	// �_�u���o�b�t�@�̐؂�ւ�
	d3d->swapChain->Present(0, 0);
}

// ������
void TitleScene::Init()
{
	// �萔�o�b�t�@�쐬
	this->CreateConstBuffer();

	// �J�����쐬
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;
	InitCamera(gpCamera);

	// ���f���ǂݍ���

	// �^�C�g���w�i�ǂݍ���
	// ���f���}�l�[�W���[�ŊǗ�
	gModelManager["title"] = mPoly->CreateSquarePolygon(
		2.0f, 2.0f, 1.0f, 1.0f, L"assets/Title/Title_test.png");

	// �X�^�[�g�{�^���ǂݍ���
	gModelManager["startButton"] = mPoly->CreateSquarePolygon(
		0.5f, 0.5f, 1.0f, 1.0f, L"assets/Title/title_Button_start.png");

	// �I���{�^���ǂݍ���
	gModelManager["quitButton"] = mPoly->CreateSquarePolygon(
		0.5f, 0.5f, 1.0f, 1.0f, L"assets/Title/title_Button_quit.png");

	// �I�u�W�F�N�g����

	// �^�C�g����ʗp�I�u�W�F�N�g����
	gObjManager["title"] = new NormalObject();
	Model* pModel = gObjManager["title"]->GetModel();
	pModel->SetModelData(gModelManager["title"]);
	pModel->Set2dRender(true);	// 2D�`��ւ̐؂�ւ�
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));

	// �X�^�[�g�{�^���p�I�u�W�F�N�g����
	gObjManager["startButton"] = new NormalObject();
	pModel = gObjManager["startButton"]->GetModel();
	pModel->SetModelData(gModelManager["startButton"]);
	pModel->Set2dRender(true);	// 2D�`��ւ̐؂�ւ�
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	pModel->mPos.x = 0.6f;
	pModel->mPos.y = 0.0f;
	// �摜�����x���グ��(�������̏�Ԃɂ���)

	// �I���{�^���p�I�u�W�F�N�g����
	gObjManager["quitButton"] = new NormalObject();
	pModel = gObjManager["quitButton"]->GetModel();
	pModel->SetModelData(gModelManager["quitButton"]);
	pModel->Set2dRender(true);	// 2D�`��ւ̐؂�ւ�
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	pModel->mPos.x = 0.6f;
	pModel->mPos.y = -0.7f;
	// �摜�����x���グ��(�������̏�Ԃɂ���)

	// �I������Ă���{�^�����w��
	selectNum = 1;

	// �Ǐ]�J�������Ǐ]����Ώۂ�ݒ�
	((BackCamera*)gpCamera)->SetTarget(gObjManager["title"]);
}

void TitleScene::InitCamera(Camera* cam)
{
	// �J���������l
// eye��focus���������W���ƃ_��
	cam->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	cam->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	// up��(0.0f,0.0f,0.0f)���ƃ_��
	cam->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
}

void TitleScene::CreateConstBuffer()
{
	// �萔�o�b�t�@�쐬
// �R���X�^���g�o�b�t�@�Ƃ��č쐬���邽�߂̏��ݒ�
	D3D11_BUFFER_DESC const_buffef_D{};
	const_buffef_D.ByteWidth = 4 * 4 * 4 * 4 * 2;				// �o�b�t�@�̃T�C�Y�i4x4�s��x4�j
	const_buffef_D.Usage = D3D11_USAGE_DYNAMIC;					// �g�p���@
	const_buffef_D.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// �o�b�t�@�̎��(�R���X�^���g�o�b�t�@)
	const_buffef_D.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// CPU�A�N�Z�X�ݒ�
	const_buffef_D.MiscFlags = 0;								// ���̑��̃I�v�V����
	const_buffef_D.StructureByteStride = 0;						// �\���̃T�C�Y(�s����g���������0�ł����삷�邱�Ƃ����؂���)

	Direct3D_Get()->device->CreateBuffer(&const_buffef_D,
		nullptr, &gpConstBuffer);
}

void TitleScene::Release()
{
	// �Q�[���I�u�W�F�N�g���폜
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		delete i->second;

	// ���f���}�l�[�W���[�����S���f�������
	for (auto i = gModelManager.begin();//�A�z�z��̐擪���
		i != gModelManager.end();//�A�z�z��̍Ō�����
		i++)
	{
		// first �c �Y����
		// second �c �v�f���̂���
		COM_SAFE_RELEASE(i->second.mSRV);//�e�N�X�`��
		COM_SAFE_RELEASE(i->second.mVertexBuffer);//���_�o�b�t�@
	}
	// �A�z�z��̑S�v�f���폜����i�v�f���O�ɂ���j
	gModelManager.clear();
}
