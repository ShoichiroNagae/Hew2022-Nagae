#include "GameScene.h"
#include "Direct3D.h"
#include <DirectXMath.h>
#include "ObjModelLoader.h"
#include "NormalObject.h"
#include "BillboardObject.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"
#include "SceneManager.h"

#include "AnimationData.h"
#include "CreateSquarePolygon.h"

RESULT_DATA BaseScene::mData;
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

 // �r���{�[�h�A�j���[�V�����֌W�n�p���[�_�[�@�@�쐬::��ˏ�
void GameScene::ModelLoad(ObjModelLoader oml, std::string ModelName,
	DirectX::XMFLOAT4 Set_whuv, const wchar_t* pTexFileName)
{
	oml = ObjModelLoader();
	gModelManager[ModelName] = oml.Load(
		Set_whuv.x, Set_whuv.y, Set_whuv.z, Set_whuv.w, pTexFileName
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
	if (playerLanded) return;

	int min = ENEMY_MIN_XPOS;
	int max = ENEMY_MAX_XPOS;
	std::random_device rnd;									// �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());									//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_int_distribution<> randDecimal(0, 99);		// [0, 99] �͈̗͂���			������
	std::uniform_int_distribution<> randInteger(min, max);	// �v���C���[�Ɍ�����͈̗͂���	������
	std::uniform_int_distribution<> randY(1, 7);	// Y���p����	������

	// �G�̈ʒu�������_���Ō���
	float EnemyPosX = randInteger(mt);
	EnemyPosX += (randDecimal(mt) * 0.01f);
	float EnemyPosY = randY(mt);

	Model* pPlayerModel = gObjManager["Player"]->GetModel();
	float EnemyPosZ = pPlayerModel->mPos.z + 50.0f;
	if (EnemyPosZ > GROUND_POS) EnemyPosZ = GROUND_POS - 50.0f;

	// �G�𐶐�
	GameObject* tmp = new NormalObject();
	Model* pEnemyModel = tmp->GetModel();
	HitSphere* pHit = tmp->GetHit();

	tmp->mSpeed = -ENEMY_SPEED_DEF;
	pEnemyModel->SetModelData(gModelManager["Enemy"]);
	pEnemyModel->SetScale(2.0f);
	pEnemyModel->mPos.z = EnemyPosZ;
	pEnemyModel->mPos.x = EnemyPosX;
	pEnemyModel->mPos.y = EnemyPosY;
	pEnemyModel->mRotate.y = 90.0f;
	pHit->SetHankei(1.0f);
	pEnemyModel->mCamera = gpCamera;
	gEnemyManager.emplace_back(tmp);
}

bool GameScene::CheckEnemy(GameObject* _enemy)
{
	Model* Enemy = _enemy->GetModel();
	bool isEnemyBack = false;

	// �G���v���C���[�������ɂ���
	if (Enemy->mPos.z < gpCamera->mEye.z) isEnemyBack = true;

	return isEnemyBack;
}

void GameScene::MoveLimit()
{
	// �v���C���[�̍��W���擾
	Model* pModel = gObjManager["Player"]->GetModel();
	float& x = pModel->mPos.x;
	float& y = pModel->mPos.y;
	float& z = pModel->mPos.z;

	// �v���C���[�ړ����E�l��ݒ�
	const float xMax = 5.5f;
	const float xMin = -5.5f;
	const float yMax = 7.5f;
	const float yMin = 0.5f;

	// ����
	if (x >= xMax) x = xMax;
	if (x <= xMin) x = xMin;
	if (y >= yMax) y = yMax;
	if (y <= yMin) y = yMin;
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
	ModelLoad(loader, "Player", Player2DSize, L"assets/Game/MainPlayer.png");		// �v���C���[(�ύX�v�])
	ModelLoad(loader, "Enemy", "assets/Game/billboard.obj", L"assets/Game/HewEnemy01.png");		// �G
	ModelLoad(loader, "Enemy2", "assets/Game/billboard.obj", L"assets/Game/HewEnemy02.png");		// �G
	ModelLoad(loader, "BackGround", "assets/Game/ground1.obj", L"assets/ground1.jpg");		// �w�i

	ModelLoad(loader, "clearLogo", 1.0f, 1.0f, 1.0f, 1.0f, L"assets/Game/clearlogo.png");	// �N���A���S

	// 2D�L�����I�u�W�F�N�g����
	gObjManager["Player"] = new BillboardObject();
	ObjectCreate("Player", 1.5f, 0.0f, 1.0f, 0.0f);
	Model* pModel = gObjManager["Player"]->GetModel();
	pModel->mRotate.y = 90.0f;	// �v���C���[��Z�������Ɍ�����
	pModel->mCamera = gpCamera;
	pModel->mPos.y = 3.0f;
	HitSphere* pHit = gObjManager["Player"]->GetHit();                                                                                                            
	pHit->SetHankei(0.4f);
	gObjManager["Player"]->mSpeed = 0.005f;
	pModel->SetUVSplit(Player2DSize); //�A�j���[�V�����pUV�̃Z�b�g(����Ȃ��Ɠ����Ȃ��B)

	// �N���A���S����
	gObjManager["clearLogo"] = new NormalObject();
	ObjectCreate("clearLogo", 1.0f, 0.0f, 0.0f, 0.0f);
	pModel = gObjManager["clearLogo"]->GetModel();
	pModel->mCamera = gpCamera;
	pModel->m2dRender = true;
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	gObjManager["clearLogo"]->mActive = false;

	//gModelManager["title"] = CreateSquarePolygon(1.0f, 0.7f, 1.0f, 1.0f, L"assets/TitleLogo640.png");
	//tmp = new NormalObject();
	//pModel = tmp->GetModel();
	//pModel->SetModelData(gModelManager["title"]);
	//pModel->Set2dRender(true);
	//pModel->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	//gObjectList.emplace_back(tmp);

	// �n�ʂ𐶐�
	pModel = nullptr;
	// ����
	for (int i = 0; i < MAX_GROUND; i++)
	{
		GameObject* tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = -4.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = -2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 0.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = -2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 4.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = -2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);
	}
	// �㑤
	for (int i = 0; i < MAX_GROUND; i++)
	{
		GameObject* tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = -4.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 10.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 0.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 10.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 4.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 10.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);
	}
	// �E��
	for (int i = 0; i < MAX_GROUND; i++)
	{
		GameObject* tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = -8.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = -8.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 6.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);
	}
	// ����
	for (int i = 0; i < MAX_GROUND; i++)
	{
		GameObject* tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 8.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 8.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 6.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);
	}
		

	// �����o�ϐ�������
	frameCount = 0;
	playerLanded = false;

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

	// ��l���̈ړ�
	Model* pPlayerModel = gObjManager["Player"]->GetModel();
	if (Input_GetKeyDown('W')) pPlayerModel->mPos.y += 0.01f;
	if (Input_GetKeyDown('S')) pPlayerModel->mPos.y -= 0.01f;
	if (Input_GetKeyDown('A')) pPlayerModel->mPos.x -= 0.01f;
	if (Input_GetKeyDown('D')) pPlayerModel->mPos.x += 0.01f;
// ************************************************************* 
	// �A�j���[�V�����؂�ւ��e�X�g
	// gObjManager����ʂ�object�ɐ؂�ւ���
	// if(��ԕϐ�)��p�ӂ��ăA�j���[�V�����̊Ǘ�������
	// ���O�ɃA�j���[�V�����pUV�̃Z�b�g�iSetUVSplit�j�����Ȃ��Ɠ����Ȃ��BInit�Őݒ�)
	// �A�j���[�V��������
	// �A�j���[�V�����̎��Ԃ��J�E���g

	

	switch (SetAnimState)
	{
	case DEFAULT:
		pPlayerModel->AnimationUpdate(PLAYER2DSTATE::DEFAULT, Char2D_DefaultFlame);
		if (Input_GetKeyTrigger(VK_SPACE)) {// �X�؁[�X�L�[�𐄂�����t���OON
			SetAnimState = PLAYER2DSTATE::ATTACK;
			pPlayerModel->mAnimTime = 0.0f;
			pPlayerModel->mAnimSpeed = 0.01f;
			pPlayerModel->Loop = false;
		}
		break;

	case ATTACK:
		pPlayerModel->AnimationUpdate(PLAYER2DSTATE::ATTACK, Char2D_AttackFlame);
		if (pPlayerModel->Loop == true)// �U���A�j���[�V�������Ō�܂ōs���ƃt���OOFF
		{
			SetAnimState = PLAYER2DSTATE::DEFAULT;
			pPlayerModel->mAnimTime = 0.0f;
			pPlayerModel->mAnimSpeed = 0.004f;
		}
		break;
	}
	//// �Œ�\��
	//pPlayerModel->SetUVAnimation(PLAYER2DSTATE::FRONT, Char2D_kihonFlame[0]);

	//// �e�N�X�`���ύX
	/*if (Input_GetKeyDown(VK_SPACE))
		p2DcharModel->ChangeTexData(L"assets/ground1.jpg");*/
// **************************************************************	
	// �v���C���[�̈ړ��𐧌�
	MoveLimit();

	// �����E����
	if (Input_GetKeyDown('R')) {
		if (gObjManager["Player"]->mSpeed < 0.01f)
		{
			gObjManager["Player"]->mSpeed += 0.0001f;
		}
		else if (gObjManager["Player"]->mSpeed < 0.05f) 
		{
			gObjManager["Player"]->mSpeed += 0.00005f + (0.0000001f / gObjManager["Player"]->mSpeed);
		}
		else
		{
			gObjManager["Player"]->mSpeed += 0.0000001f / gObjManager["Player"]->mSpeed;
		}
	}

	if (Input_GetKeyDown('F')) {
		if (gObjManager["Player"]->mSpeed > 0.005f)
			gObjManager["Player"]->mSpeed -= 0.0001f;
	}

	// �G�̎�������
	if (frameCount == 50) CreateEnemy();
	if (frameCount > 50) frameCount = 0;

	// �w�i�e�X�g
	if (Input_GetKeyTrigger('L')) {
		// �I�u�W�F�N�g����
		gObjManager["BackGround"] = new NormalObject();
		Model* pModel = gObjManager["BackGround"]->GetModel();
		pModel->SetModelData(gModelManager["BackGround"]);
		pModel->SetScale(100.0f);
		pModel->mPos.x = -100.0f;
		pModel->mPos.y = 1.0f;
		pModel->mPos.z = 500.0f;
		pModel->mCamera = gpCamera;
	}

	// �Q�[���I�u�W�F�N�g���X�V
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Update();

	// �G��S�čX�V
	for (int i = 0; i < gEnemyManager.size(); i++)
	{
		GameObject* tmp = gEnemyManager[i];
		HitSphere* tmpHit = tmp->GetHit();

		tmp->Update();

		// �����蔻������s
		if (tmpHit->IsHit(gObjManager["Player"]->GetHit()))
		{
			// �������Ă���Ƃ��Ƀ{�^�����͂���������G������
			if (SetAnimState == PLAYER2DSTATE::ATTACK)
			{
				// �v���C���[�̐؂�A�j���[�V�����Ƃ����s�H

				// �X�R�A�����H
				mData.KILL_ENEMY++;	// �|�����G�̐��𑝉�
				this->nowCombo++;	// �R���{�𑝉�

				 delete gEnemyManager[i];
				 gEnemyManager.erase(gEnemyManager.begin() + i);
			}
			// �G�ɓ����Ă��܂����Ƃ�
			if(SetAnimState == PLAYER2DSTATE::DEFAULT)
			{
				// ������ɐ������
				Model* pEnemyModel = tmp->GetModel();
				if (pPlayerModel->mPos.z > pEnemyModel->mPos.z + 1.0f) {
					if (gObjManager["Player"]->mSpeed > 0.0f)
					{
						gObjManager["Player"]->mSpeed = 0.001f;
						delete gEnemyManager[i];
						gEnemyManager.erase(gEnemyManager.begin() + i);
					}
				}
				// �v���C���[�̃X�R�A�Ƃ����炷�H

				// �R���{��0�ɖ߂�
				if (nowCombo > maxCombo) maxCombo = nowCombo;
				this->nowCombo = 0;
			}
		}
		// �v���C���[�ƓG���ڐG���Ă��炸
		// �G���v���C���[�̌��ɍs���������
		else if (CheckEnemy(gEnemyManager[i]))
		{
			if (nowCombo > maxCombo) maxCombo = nowCombo;
			this->nowCombo = 0;

			delete gEnemyManager[i];
			gEnemyManager.erase(gEnemyManager.begin() + i);
		}
	}
	

	// �e�Ǘ��z��̒��g�����ׂčX�V����
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Update();

	// �n�ʂ�S�čX�V
	for (int i = 0; i < gGround.size(); i++)
		gGround[i]->Update();

	// �J�����̍X�V�����i�r���[�ϊ��s��v�Z�j
	gpCamera->Update();

	// �v���C���[���n�ʂɂ����Ƃ��̏���
	if (gObjManager["Player"]->GetModel()->mPos.z > GROUND_POS)
	{
		// �v���C���[�̈ړ����~�߂�
		gObjManager["Player"]->mSpeed = 0.0f;
		playerLanded = true;

		// �N���A���S��\��
		gObjManager["clearLogo"]->mActive = true;

		// �N���A���S�\�����牽�b�ォ�Ƀ��U���g�V�[���֑J��
	}

	// �t���[�������Z
	frameCount++;

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

	// �n�ʂ�S�ĕ`��
	for (int i = 0; i < gGround.size(); i++)
		gGround[i]->Draw();
	
	// �G��S�ĕ`��
	for (int i = 0; i < gEnemyManager.size(); i++)
		gEnemyManager[i]->Draw();

	// �Q�[���I�u�W�F�N�g��`��
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Draw();

	// �G��S�ĕ`��
	for (int i = 0; i < gEnemyManager.size(); i++)
		gEnemyManager[i]->Draw();

	

	// �e�Ǘ��z��̒��g�����ׂĕ`�悷��
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Draw();

	// �_�u���o�b�t�@�̐؂�ւ�
	d3d->swapChain->Present(0, 0);
}

// ���
void GameScene::Release()
{
	// �n�ʂ�S�ĕ`��
	for (int i = 0; i < gGround.size(); i++)
		delete gGround[i];

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
