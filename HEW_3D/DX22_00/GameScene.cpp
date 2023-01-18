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

// �G�̎�������
void GameScene::CreateEnemy()
{
	int min = ENEMY_MIN_XPOS;
	int max = ENEMY_MAX_XPOS;
	std::random_device rnd;									// �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());									//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_int_distribution<> randDecimal(0, 99);		// [0, 99] �͈̗͂���
	std::uniform_int_distribution<> randInteger(min, max);	// �v���C���[�Ɍ�����͈̗͂���

	// �G�̈ʒu�������_���Ō���
	float EnemyPosX = randInteger(mt);
	EnemyPosX += (randDecimal(mt) * 0.01f);

	// �G�𐶐�
	GameObject* tmp = new NormalObject();
	Model* pEnemyModel = tmp->GetModel();
	tmp->mSpeed = -ENEMY_SPEED_DEF;
	pEnemyModel->SetModelData(gModelManager["Enemy"]);
	pEnemyModel->SetScale(1.0f);
	pEnemyModel->mPos.z = 10.0f;
	pEnemyModel->mPos.x = EnemyPosX;
	pEnemyModel->mRotate.y = 90.0f;
	pEnemyModel->mCamera = gpCamera;
	gEnemyManager.emplace_back(tmp);
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
	ModelLoad(loader, "ground1", "assets/Game/ground1.obj", L"assets/Game/ground1.jpg");	// �n��
	ModelLoad(loader, "Player", 0.5f, 0.6f, 0.33f, 0.25f, L"assets/Game/char01.png");		// �v���C���[
	ModelLoad(loader, "Enemy", "assets/Game/billboard.obj", L"assets/Game/Enemy.png");		// �G


	// 2D�L�����I�u�W�F�N�g����
	gObjManager["Player"] = new BillboardObject();
	ObjectCreate("Player", 1.5f, 0.0f, 1.0f, 0.0f);
	Model* pModel = gObjManager["Player"]->GetModel();
	pModel->mRotate.y = 90.0f;	// �v���C���[��Z�������Ɍ�����
	pModel->mCamera = gpCamera;

	// 2D�L�����I�u�W�F�N�g����
	gObjManager["Enemy"] = new NormalObject();
	ObjectCreate("Enemy", 1.5f, 0.0f, 1.0f, 10.0f);

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
	((BackCamera*)gpCamera)->SetTarget(gObjManager["Player"]);
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
	//gObjManager["Player"]->mSpeed = 0.001f;


	// ��l���̈ړ�
	Model* pPlayerModel = gObjManager["Player"]->GetModel();
	if (Input_GetKeyDown('W')) pPlayerModel->mPos.y += 0.001f;
	if (Input_GetKeyDown('S')) pPlayerModel->mPos.y -= 0.001f;
	if (Input_GetKeyDown('A')) pPlayerModel->mPos.x -= 0.001f;
	if (Input_GetKeyDown('D')) pPlayerModel->mPos.x += 0.001f;
	// �����E����				
	if (Input_GetKeyDown('R')) pPlayerModel->mPos.z -= 0.001f;
	if (Input_GetKeyDown('F')) pPlayerModel->mPos.z += 0.001f;


	// �G�̎�������(��)
	if (Input_GetKeyTrigger(VK_SPACE))
	{
		CreateEnemy();
	}
	// �w�i�e�X�g
	if (Input_GetKeyTrigger('L')) {
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

	// �G��S�čX�V
	for (int i = 0; i < gEnemyManager.size(); i++)
		gEnemyManager[i]->Update();

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


	// �G��S�ĕ`��
	for (int i = 0; i < gEnemyManager.size(); i++)
		gEnemyManager[i]->Draw();

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

	// �G�}�l�[�W���[�̗v�f��S�ĊJ��
	for (int i = 0; i < gEnemyManager.size(); i++)
		delete gEnemyManager[i];
}

// �f�X�g���N�^
GameScene::~GameScene() {
	Release();
}
