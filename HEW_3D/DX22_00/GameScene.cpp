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

extern ID3D11Buffer* gpConstBuffer; //�萔�o�b�t�@

void GameScene::Init()
{	
	// �萔�o�b�t�@�쐬
	// �R���X�^���g�o�b�t�@�Ƃ��č쐬���邽�߂̏��ݒ�
	D3D11_BUFFER_DESC contstat_buffer_desc{};
	contstat_buffer_desc.ByteWidth = 4 * 4 * 4 * 4 *4;	// �o�b�t�@�̃T�C�Y�i4x4�s��x4�j
	contstat_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;		// �g�p���@
	contstat_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �o�b�t�@�̎��(�R���X�^���g�o�b�t�@)
	contstat_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPU�A�N�Z�X�ݒ�
	contstat_buffer_desc.MiscFlags = 0;				// ���̑��̃I�v�V����
	contstat_buffer_desc.StructureByteStride = 0;			// �\���̃T�C�Y(�s����g���������0�ł����삷�邱�Ƃ����؂���)

	Direct3D_Get()->device->CreateBuffer(&contstat_buffer_desc,
		nullptr, &gpConstBuffer);

	// �J�����쐬
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;

	// �J���������l
	// eye��focus���������W���ƃ_��
	gpCamera->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	gpCamera->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	// up��(0.0f,0.0f,0.0f)���ƃ_��
	gpCamera->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));

	// �R�e�[�W���f���ǂݍ���
	ObjModelLoader loader;
	gModelManager["cottage"] = loader.Load(
		"assets/cottage.obj", L"assets/cottage.png");
	// �R�e�[�W�pModel�I�u�W�F�N�g����
	gObjManager["cottage"] = new NormalObject();
	Model* pModel = gObjManager["cottage"]->GetModel();
	pModel->SetModelData(gModelManager["cottage"]); // 3D�f�[�^���Z�b�g
	pModel->SetScale(0.001f);
	pModel->mPos.z = 4.0f;
	pModel->mPos.y = 0.0f;
	pModel->mCamera = gpCamera;

	// �e���f���ǂݍ���
	loader = ObjModelLoader();
	gModelManager["gun"] = loader.Load(
		"assets/gun.obj", L"assets/gun.png");
	// �e�pModel�I�u�W�F�N�g����
	gObjManager["gun"] = new NormalObject();
	pModel = gObjManager["gun"]->GetModel();
	pModel->SetModelData(gModelManager["gun"]);
	pModel->SetScale(1.5f);
	pModel->mPos.z = 0.0f;
	pModel->mPos.y = 0.8f;
	pModel->mPos.x = 0.0f;
	pModel->mRotate.y = 0.0f;
	pModel->mCamera = gpCamera;

	// �n�ʃ��f���ǂݍ���
	loader = ObjModelLoader();
	gModelManager["ground1"] = loader.Load(
		"assets/ground1.obj", L"assets/ground1.jpg"
	);
	// �R�e�[�W���f���I�u�W�F�N�g����
	gObjManager["cottage"] = new NormalObject();
	pModel = gObjManager["cottage"]->GetModel();
	pModel->SetModelData(gModelManager["cottage"]);
	pModel->SetScale(1.5f);
	pModel->mPos.z = 0.0f;
	pModel->mPos.y = 0.8f;
	pModel->mPos.z = 0.0f;
	pModel->mRotate.y = 0.0f;
	pModel->mCamera = gpCamera;
	// �e�i�r���{�[�h�j�p���f���ǂݍ���
	loader = ObjModelLoader();
	gModelManager["shot"] = loader.Load(
		"assets/billboard.obj", L"assets/shot.png"
	);

	//// 2D�L�������f���ǂݍ���
	loader = ObjModelLoader();
	gModelManager["2Dchar"] = loader.Load(
		1.0f, 1.2f, 0.33f, 0.25f, L"assets/char01.png");

	// 2D�L�����I�u�W�F�N�g����
	gObjManager["2Dchar"] = new BillboardObject();
	pModel = gObjManager["2Dchar"]->GetModel();
	pModel->SetModelData(gModelManager["2Dchar"]);
	pModel->SetScale(1.0f);
	pModel->mPos.x = -10.0f;
	pModel->mPos.y = 1.0f;
	pModel->mPos.z = 0.8f;
	pModel->mCamera = gpCamera;

	// �n�ʂ𐶐�
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j] = new NormalObject();
			Model* pGroundModel = gpGround[i][j]->GetModel();
			pGroundModel->SetModelData(gModelManager["ground1"]);
			pGroundModel->SetScale(1.0f);
			pGroundModel->mPos.x = -10.0f + 2.0f * j;
			pGroundModel->mPos.z = -10.0f + 2.0f * i;
			pGroundModel->mPos.y = -1.0f;
			pGroundModel->mCamera = gpCamera;
		}
	}

	// �Ǐ]�J�������Ǐ]����Ώۂ�ݒ�
	((BackCamera*)gpCamera)->SetTarget(gObjManager["gun"]);
}

// ������
GameScene::GameScene(){
	Init();
}

// ���
GameScene::~GameScene(){
	Release();
}

void GameScene::Update()
{
	// �f���^�^�C�����z��O�̒l�ɂȂ����ꍇ�̏���
	if (gDeltaTime >= 100)
	{
		gDeltaTime = 0;
	}
	
	if (gDeltaTime <= 0)
	{
		gDeltaTime = 1;
	}

	// �L�����N�^�[�ړ�
	// ���@�L�����N�^�[�������Ă�������ɐi�܂���ɂ́H
	// �@���@���i�K�ňړ��ł���
	// �@���@�u�O�����x�N�g���v���g��
	gObjManager["gun"]->mSpeed = 0.0f;

	// �e�̑O�i
	gObjManager["gun"]->mSpeed = 0.001f;

	// �e�̈ړ�
	Model* pGunModel = gObjManager["gun"]->GetModel();
	if (Input_GetKeyDown('W'))
		pGunModel->mPos.y += 0.001f;

	if (Input_GetKeyDown('S'))
		pGunModel->mPos.y -= 0.001f;

	if (Input_GetKeyDown('A'))
		pGunModel->mPos.z -= 0.001f;

	if (Input_GetKeyDown('D'))
		pGunModel->mPos.z += 0.001f;
	// �����E����
	if (Input_GetKeyDown('R'))
		pGunModel->mPos.x -= 0.001f;

	if (Input_GetKeyDown('F'))
		pGunModel->mPos.x += 0.001f;

// ************************************************************* 
	// �A�j���[�V�����؂�ւ��e�X�g
	// gObjManager����ʂ�object�ɐ؂�ւ���
	// 
	//// �����ύX
	//BillboardModel* p2DcharModel = gObjManager["2Dchar"]->GetModel();

	/*if (Input_GetKeyDown(VK_SPACE))
		p2DcharModel->ChangeTexData(L"assets/ground1.jpg");*/
// **************************************************************	

	// �Q�[���I�u�W�F�N�g��`��
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Update();

	// �e�Ǘ��z��̒��g�����ׂčX�V����
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Update();

	// �n�ʂ�S�čX�V
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j]->Update();
		}
	}

	// �J�����̍X�V�����i�r���[�ϊ��s��v�Z�j
	gpCamera->Update();
}

void GameScene::Draw()
{
	// DIRECT3D�\���̂ɃA�N�Z�X����
	DIRECT3D* d3d = Direct3D_Get();

	// ��ʃN���A�i�w��F�œh��Ԃ��j
	// �h��Ԃ��F�̎w��i�����F0.0f�`1.0f�j
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Z�o�b�t�@�A�X�e���V���o�b�t�@�N���A
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// �f�v�X�X�e���V���r���[
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// �y�o�b�t�@��1.0�ŃN���A����(0.0f�`1.0f)
		0);				// �X�e���V���o�b�t�@��0�ŃN���A����

	// ���@���O�̕`�揈���������ɏ��� *******
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j]->Draw();
		}
	}

	// �Q�[���I�u�W�F�N�g��`��
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Draw();

	// �e�Ǘ��z��̒��g�����ׂĕ`�悷��
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Draw();

	// �_�u���o�b�t�@�̐؂�ւ�
	d3d->swapChain->Present(0, 0);
}

void GameScene::Release()
{
	// �e�Ǘ��z��̒��g�����ׂč폜����


	// �n�ʂ̗v�f�����ׂč폜����
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			delete gpGround[i][j];
		}
	}

	COM_SAFE_RELEASE(gpConstBuffer);

	// ���f���}�l�[�W���[���Ǘ����郂�f����S�������
	for (auto i = gModelManager.begin();// ��ԑO�̗v�f�̊Ǘ�����Ԃ�
		i != gModelManager.end();// ��ԍŌ�̊Ǘ�����Ԃ�
		i++)
	{
		// first �c �Y����
		// second �c �i�[����Ă���v�f���̂���
		COM_SAFE_RELEASE(i->second.mSRV);
		COM_SAFE_RELEASE(i->second.mVertexBuffer);
	}

	// �A�z�z��̗v�f��S�폜
	gModelManager.clear();

	// �I�u�W�F�N�g�}�l�[�W���[���Ǘ�����I�u�W�F�N�g��S�J������
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
	{
		delete i->second;
	}
	// �A�z�z��̗v�f��S�폜
	gObjManager.clear();
}
