#include "GameScene.h"
#include "Direct3D.h"
#include <DirectXMath.h>
#include "ObjModelLoader.h"
#include "NormalObject.h"
#include "BillboardObject.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"

#include "AnimationData.h"

#include "CreateSquarePolygon.h"

extern ID3D11Buffer* gpConstBuffer; //�萔�o�b�t�@

// �R���X�g���N�^
GameScene::GameScene() {
	Init();
}

// �萔�o�b�t�@�쐬
void GameScene::CreateConstBuffer()
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

// �J���������ݒ�
void GameScene::CameraInit(Camera* cam)
{
	// �J���������l
	// eye��focus���������W���ƃ_��
	cam->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	cam->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	// up��(0.0f,0.0f,0.0f)���ƃ_��
	cam->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
}

// ���f���ǂݍ���
void GameScene::ModelLoad(ObjModelLoader oml, std::string ModelName,
	const char* pObjFileName, const wchar_t* pTexFileName)
{
	oml = ObjModelLoader();
	gModelManager[ModelName] = oml.Load(
		pObjFileName, pTexFileName
	);
}

void GameScene::ModelLoad(ObjModelLoader oml,std::string ModelName,
	float width, float height, float uvWidth, float uvHeight, const wchar_t* pTexFileName)
{
	oml = ObjModelLoader();
	gModelManager[ModelName] = oml.Load(
		width, height, uvWidth, uvHeight, pTexFileName
	);
}

void GameScene::ObjectCreate(std::string objName, float mScale, float mx, float my, float mz)
{
	Model* pModel = gObjManager[objName]->GetModel();
	pModel->SetModelData(gModelManager[objName]);
	pModel->SetScale(mScale);
	pModel->Setpos(mx, my, mz);
	pModel->mCamera = gpCamera;
}

// ������
void GameScene::Init()
{	
	// �萔�o�b�t�@�쐬
	this->CreateConstBuffer();

	// �J�����쐬
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;
	CameraInit(gpCamera);

	// ���f���ǂݍ���
	ModelLoad(loader, "ground1", "assets/ground1.obj", L"assets/ground1.jpg");
	ModelLoad(loader, "gun", "assets/gun.obj", L"assets/gun.png");
	ModelLoad(loader, "cottage", "assets/cottage.obj", L"assets/cottage.png");
	ModelLoad(loader, "shot", "assets/billboard.obj", L"assets/shot.png");
	ModelLoad(loader, "2Dchar", 1.0f, 1.2f, 0.33f, 0.25f, L"assets/char01.png");
////// 2D�L�������f���ǂݍ���
	////GameScene.h�ɃT�C�Y�ϐ����`����
	//gModelManager["2Dchar"] = loader.Load(
	//	CHAR2DSize, L"assets/char01.png");

	// �R�e�[�W�pModel�I�u�W�F�N�g����
	gObjManager["cottage"] = new NormalObject();
	ObjectCreate("cottage", 0.001f, -10.0f, -1.0f, 0.0f);

	// �e�pModel�I�u�W�F�N�g����
	gObjManager["gun"] = new NormalObject();
	ObjectCreate("gun", 1.5f, 0.0f, 1.0f, 0.0f);

	// 2D�L�����I�u�W�F�N�g����
	gObjManager["2Dchar"] = new BillboardObject();
	ObjectCreate("2Dchar", 1.0f, -10.0f, 1.0f, 0.8f);

	// �n�ʂ𐶐�
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j] = new NormalObject();
			Model* pGroundModel = gpGround[i][j]->GetModel();
			pGroundModel->SetModelData(gModelManager["ground1"]);
			pGroundModel->SetScale(1.0f);
			pGroundModel->Setpos(-10.0f + 2.0f * j, -2.0f, -10.0f + 2.0f * i);
			pGroundModel->mCamera = gpCamera;
		}
	}

	// �Ǐ]�J�������Ǐ]����Ώۂ�ݒ�
	((BackCamera*)gpCamera)->SetTarget(gObjManager["gun"]);
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
	// ���@���i�K�ňړ��ł���
	// ���u�O�����x�N�g���v���g��
	gObjManager["gun"]->mSpeed = 0.0f;

	// �e�̑O�i
	gObjManager["gun"]->mSpeed = 0.001f;

	// ��l���̈ړ�
	Model* pGunModel = gObjManager["gun"]->GetModel();
	if (Input_GetKeyDown('W')) pGunModel->mPos.y += 0.001f;
	if (Input_GetKeyDown('S')) pGunModel->mPos.y -= 0.001f;
	if (Input_GetKeyDown('A')) pGunModel->mPos.z -= 0.001f;
	if (Input_GetKeyDown('D')) pGunModel->mPos.z += 0.001f;
	// �����E����
	if (Input_GetKeyDown('R')) pGunModel->mPos.x -= 0.001f;
	if (Input_GetKeyDown('F')) pGunModel->mPos.x += 0.001f;

	
	// �G�̎�������(��)
	// 
	if (Input_GetKeyDown('P')) {
		ModelLoad(loader, "2Dchar2", 1.0f, 1.2f, 0.33f, 0.25f, L"assets/char01.png");
		// 2D�L�����I�u�W�F�N�g����
		gObjManager["2Dchar2"] = new BillboardObject();
		Model*  pModel = gObjManager["2Dchar2"]->GetModel();
		pModel->SetModelData(gModelManager["2Dchar2"]);
		pModel->SetScale(1.0f);
		pModel->mPos.x = -9.0f;
		pModel->mPos.y = 1.0f;
		pModel->mPos.z = 0.8f;
		pModel->mCamera = gpCamera;
	}

	// �w�i�e�X�g
	if (Input_GetKeyDown('L')) {
		ModelLoad(loader, "BackGround", 1000.0f, 1000.0f, 1.0f, 1.0f, L"assets/ground1.jpg");
		// �I�u�W�F�N�g����
		gObjManager["BackGround"] = new BillboardObject();
		Model* pModel = gObjManager["BackGround"]->GetModel();
		pModel->SetModelData(gModelManager["BackGround"]);
		pModel->SetScale(1.0f);
		pModel->mPos.x = -100.0f;
		pModel->mPos.y = 1.0f;
		pModel->mPos.z = 0.8f;
		pModel->mCamera = gpCamera;
	}

// ************************************************************* 
	// �A�j���[�V�����؂�ւ��e�X�g
	// gObjManager����ʂ�object�ɐ؂�ւ���

	//// if(��ԕϐ�)��p�ӂ��ăA�j���[�V�����̊Ǘ�������
	//Model* p2DcharModel = gObjManager["2Dchar"]->GetModel();
	//p2DcharModel->SetUVSplit(CHAR2DSize); //�A�j���[�V�����pUV�̃Z�b�g(����Ȃ��Ɠ����Ȃ��BInit�Őݒ肪��������)
	//// �A�j���[�V��������
	//p2DcharModel->AnimationUpdate(CHAR2DSTATE::BACK, Char2D_kihonFlame);

	//// �Œ�\��
	//p2DcharModel->SetUVAnimation(UP, Char2D_kihonFlame[0]);

	//// �e�N�X�`���ύX
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

// ���
void GameScene::Release()
{
	// �e�Ǘ��z��̒��g�����ׂč폜����


	// �n�ʂ̗vs�f�����ׂč폜����
	for (int i = 0; i < MAX_GROUND; i++) {
		for (int j = 0; j < MAX_GROUND; j++) {
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

// �f�X�g���N�^
GameScene::~GameScene() {
	Release();
}
