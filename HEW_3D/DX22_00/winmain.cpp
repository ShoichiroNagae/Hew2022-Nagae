#undef UNICODE  // Unicode�ł͂Ȃ��A�}���`�o�C�g�������g��

#include <Windows.h>
#include "Direct3d.h"
#include "input.h"
#include <DirectXMath.h>
#include "WICTextureLoader.h"
#include "ObjModelLoader.h"
#include "Model.h"
#include "BackCamera.h"
#include "GameObject.h"
#include "NormalObject.h"
#include "BillboardObject.h"
#include "CreateSquarePolygon.h"
#include "HitCheck_2D.h"
#include <map>  // �A�z�z��
#include <vector>
#include <xstring>


#pragma comment (lib, "winmm.lib") // timeGetTime�֐��̂���

// �}�N����`
#define CLASS_NAME    "DX21Smpl"// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME  "�X�P���g���v���O����"// �E�B���h�E�̖��O

#define SCREEN_WIDTH (1024)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT (576+30)	// �E�C���h�E�̍���

// �\���̂̒�`

// �֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �Q�[���̉�ʕ`��̊֐�
void Game_Init();
void Game_Draw();
void Game_Update();
void Game_Release();


// �O���[�o���ϐ��̐錾

// ���_�o�b�t�@�p�̕ϐ�
ID3D11Buffer* gpVertexBuffer;

// ���_�������ϐ�
int gNumVertex;

// �e�N�X�`���p�̕ϐ�
ID3D11ShaderResourceView* gpTextureHalOsaka; // HAL OSAKA

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

#define MAX_GROUND  50
// ���P�O�~�c�P�O�̓񎟌��z��
GameObject* gpGround1[MAX_GROUND];
GameObject* gpGround2[MAX_GROUND];
GameObject* gpGround3[MAX_GROUND];
GameObject* gpGround4[MAX_GROUND];

// �e�}�l�[�W���[
std::vector<GameObject*> gShotManager;

// ���f���}�l�[�W���[
// �A�z�z�񁁓Y�����𐮐��ȊO�ɂł���z��
std::map<std::string, ModelData> gModelManager;

// �I�u�W�F�N�g�}�l�[�W���[ �A�z�z��ō쐬
std::map <std::string, GameObject*> gObjectManager;

// �J�����N���X�̕ϐ�
Camera* gpCamera;

// �f���^�^�C���p�̕ϐ�
DWORD gDeltaTime;

// �����蔻��p�̕ϐ�
HitCheck_2D* gpHit;

// WinMain�֐������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc; // WND = Window

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc); // Windows�ɃE�C���h�E����o�^

	HWND hWnd; // H=Handle=�|�C���^ WND=Window
	hWnd = CreateWindowEx(0,// �g���E�B���h�E�X�^�C��
		CLASS_NAME,// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,// �E�B���h�E�̖��O
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,// �E�B���h�E�̍���w���W
		CW_USEDEFAULT,// �E�B���h�E�̍���x���W 
		SCREEN_WIDTH,// �E�B���h�E�̕�
		SCREEN_HEIGHT,// �E�B���h�E�̍���
		NULL,// �e�E�B���h�E�̃n���h��
		NULL,// ���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,// �C���X�^���X�n���h��
		NULL);// �E�B���h�E�쐬�f�[�^

	// �w�肳�ꂽ�E�B���h�E�̕\����Ԃ�ݒ�(�E�B���h�E��\��)
	ShowWindow(hWnd, nCmdShow);
	// �E�B���h�E�̏�Ԃ𒼂��ɔ��f(�E�B���h�E�̃N���C�A���g�̈���X�V)
	UpdateWindow(hWnd);

	// Direct3D�̏������֐����Ăяo��
	Direct3D_Init(hWnd);

	Game_Init();

	

	MSG msg;
	// ���C�����[�v
	for (;;) {
		// �O��̃��[�v���烆�[�U�[���삪�����������ׂ�
		BOOL doesMessageExist = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		if (doesMessageExist)
		{
			// �ԐړI�ɃE�C���h�E�v���V�[�W�����Ăяo��
			DispatchMessage(&msg);

			// �A�v���I�����߂�����
			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			// �f���^�^�C���̌v��
			static DWORD lastTime = timeGetTime(); // �O��v�����̎���
			timeBeginPeriod(1); // ���x���P�~���b�ɏグ��
			DWORD nowTime = timeGetTime(); // ���݂̎���
			timeEndPeriod(1); // ���x�����ɖ߂�
			gDeltaTime = nowTime - lastTime; // �������f���^�^�C��
			lastTime = nowTime; // �O��v�����ԂƂ��ĕۑ�

			// �Q�[�����[�v
			Game_Update(); // �Q�[������
			Game_Draw();   // �Q�[���`��
			Input_Refresh(); // �L�[��Ԃ̍X�V
		}
	}

	Game_Release();
	// Direct3D�̉���֐����Ăяo��
	Direct3D_Release();

	UnregisterClass(CLASS_NAME, hInstance);

	return (int)msg.wParam;
}


// �E�C���h�E�v���V�[�W���֐������
// ���֐�������̂̓O���[�o���̈�(=�ǂ̊֐��̒��ł��Ȃ��ꏊ)�����I
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// uMsg�i���̊֐��̑�2�����j���A���[�U�[�����ID���
	switch (uMsg)
	{
	case WM_DESTROY:// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);// �gWM_QUIT�h���b�Z�[�W�𑗂�@���@�A�v���I��
		break;

	case WM_CLOSE:  // x�{�^���������ꂽ��
		DestroyWindow(hWnd);  // �gWM_DESTROY�h���b�Z�[�W�𑗂�
		break;

	case WM_LBUTTONDOWN: // ���N���b�N���ꂽ�Ƃ�
		break;

	case WM_RBUTTONDOWN: // �E�N���b�N���ꂽ�Ƃ�
		break;

	case WM_MOUSEMOVE: // �}�E�X�J�[�\�����������Ƃ�
		break;

	case WM_KEYDOWN:
		// �L�[�������ꂽ���̃��A�N�V����������
		// ESC�������ꂽ�̂��ǂ����`�F�b�N
		if (LOWORD(wParam) == VK_ESCAPE)
		{
			// ���b�Z�[�W�{�b�N�X�ŏC���m�F
			int result;
			result = MessageBox(NULL, "�I�����Ă�낵���ł����H",
				"�I���m�F", MB_YESNO | MB_ICONQUESTION);
			if (result == IDYES) // �u�͂��v�{�^���������ꂽ��
			{
				// x�{�^���������ꂽ�̂Ɠ������ʂ𔭊�����
				PostMessage(hWnd, WM_CLOSE, wParam, lParam);
			}
		}
		Input_SetKeyDown(LOWORD(wParam));
		break;

	case WM_KEYUP: // �L�[�������ꂽ�C�x���g
		Input_SetKeyUp(LOWORD(wParam));
		break;

	default:
		// ���case�ȊO�̏ꍇ�̏��������s
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

void Game_Init()
{
	// �萔�o�b�t�@�쐬
	// �R���X�^���g�o�b�t�@�Ƃ��č쐬���邽�߂̏��ݒ�
	D3D11_BUFFER_DESC contstat_buffer_desc;
	contstat_buffer_desc.ByteWidth = 4 * 4 * 4 * 4;	// �o�b�t�@�̃T�C�Y�i4x4�s��x4�j
	contstat_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;		// �g�p���@
	contstat_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �o�b�t�@�̎��(�R���X�^���g�o�b�t�@)
	contstat_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPU�A�N�Z�X�ݒ�
	contstat_buffer_desc.MiscFlags = 0;				// ���̑��̃I�v�V����
	contstat_buffer_desc.StructureByteStride = 0;			// �\���̃T�C�Y(�s����g���������0�ł����삷�邱�Ƃ����؂���)

	Direct3D_Get()->device->CreateBuffer(&contstat_buffer_desc,
		nullptr, &gpConstBuffer);

	// �J�����쐬
	gpCamera = new BackCamera();
	// �J���������l
	// eye��focus���������W���ƃ_��
	gpCamera->SetEye(XMFLOAT3(0.0f, 0.0f, -2.0f));
	gpCamera->SetFocus(XMFLOAT3(0.0f, 0.0f, 0.0f));
	// up��(0.0f,0.0f,0.0f)���ƃ_��
	gpCamera->SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));

	// �R�e�[�W���f���ǂݍ���
	ObjModelLoader loader;
	gModelManager["cottage"] = loader.Load(
		"assets/cottage.obj",
		L"assets/cottage.png");

	// �e���f���ǂݍ���
	ObjModelLoader loader2;
	gModelManager["gun"] = loader2.Load(
		"assets/gun.obj", L"assets/gun.png");

	// �n�ʃ��f���ǂݍ���
	ObjModelLoader loader3;
	gModelManager["ground1"] = loader3.Load(
		"assets/ground1.obj", L"assets/ground1.jpg"
	);

	// �e�i�r���{�[�h�j�p���f���ǂݍ���
	ObjModelLoader loader4;
	gModelManager["shot"] = loader4.Load(
		"assets/billboard.obj", L"assets/shot.png"
	);

	// 2D�L�������f���쐬
	gModelManager["2Dchar"] =
	CreateSquarePolygon(1.0f, 1.2f, 0.33f, 0.25f, L"assets/char01.png");

	// gObjectManager
	// �R�e�[�W�pModel�I�u�W�F�N�g����
	//gObjectManager["cottage"] = new NormalObject();
	//Model* pModel = gObjectManager["cottage"]->GetModel();
	//pModel->SetModelData(gModelManager["cottage"]); // 3D�f�[�^���Z�b�g
	//pModel->SetScale(0.001f);
	//pModel->mPos.z = 4.0f;
	//pModel->mPos.y = 0.0f;
	//pModel->mCamera = gpCamera;

	// �e�pModel�I�u�W�F�N�g����
	gObjectManager["gun"] = new NormalObject();
	Model* pModel = gObjectManager["gun"]->GetModel();
	pModel->SetModelData(gModelManager["gun"]);
	pModel->SetScale(1.5f);
	pModel->mPos.z = 0.0f;
	pModel->mPos.y = 0.3f;
	pModel->mPos.x = 0.0f;
	pModel->mRotate.y = 0.0f;
	pModel->mCamera = gpCamera;

	// 2D�L�����I�u�W�F�N�g����
	gObjectManager["2Dchar"] = new BillboardObject();
	pModel = gObjectManager["2Dchar"]->GetModel();
	pModel->SetModelData(gModelManager["2Dchar"]);
	pModel->SetScale(1.0f);
	pModel->mPos.x = -10.0f;
	pModel->mPos.y = 1.0f;
	pModel->mPos.z = 0.8f;
	pModel->mCamera = gpCamera;


	// �n�ʂ𐶐�
	for (int i = 0; i < MAX_GROUND; i++)
	{
		gpGround1[i] = new NormalObject();
		Model* pGroundModel = gpGround1[i]->GetModel();
		pGroundModel->SetModelData(gModelManager["ground1"]);
		pGroundModel->SetScale(1.0f);
		pGroundModel->mPos.x = 0.0f - 2.0f * i;
		pGroundModel->mPos.z = 0.0f;
		pGroundModel->mPos.y = -1.0f;
		pGroundModel->mCamera = gpCamera;
	}

	// �Ǐ]�J�������Ǐ]����Ώۂ�ݒ�
	((BackCamera*)gpCamera)->SetTarget(gObjectManager["gun"]);

	// �����蔻��
	gpHit = new HitCheck_2D();
	gpHit->Init();
}



void Game_Draw()
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
	for (int i = 0; i < MAX_GROUND; i++)
	{
		gpGround1[i]->Draw();
	}

	// �Q�[���I�u�W�F�N�g��`��
	for (auto i = gObjectManager.begin();
		i != gObjectManager.end();
		i++)
		i->second->Draw();

	// �e�Ǘ��z��̒��g�����ׂĕ`�悷��
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Draw();

	// �_�u���o�b�t�@�̐؂�ւ�
	d3d->swapChain->Present(0, 0);
}

void Game_Update()
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
	gObjectManager["gun"]->mSpeed = 0.0f;

	// �e�̑O�i
		gObjectManager["gun"]->mSpeed = 0.001f;

	// �e�̈ړ�
		Model* pModel = gObjectManager["gun"]->GetModel();
		if (Input_GetKeyDown('W'))
			pModel->mPos.y += 0.001f;

		if (Input_GetKeyDown('S'))
			pModel->mPos.y -= 0.001f;

		if (Input_GetKeyDown('A'))
			pModel->mPos.z -= 0.001f;

		if (Input_GetKeyDown('D'))
			pModel->mPos.z += 0.001f;

	// �Q�[���I�u�W�F�N�g��`��
	for (auto i = gObjectManager.begin();
		i != gObjectManager.end();
		i++)
		i->second->Update();

	// �e�Ǘ��z��̒��g�����ׂčX�V����
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Update();

	gpHit->set_Position(gObjectManager["gun"]->GetModel()->mPos, gObjectManager["2Dchar"]->GetModel()->mPos);
	gpHit->set_Size(gObjectManager["gun"]->GetModel()->mScale.x, gObjectManager["2Dchar"]->GetModel()->mScale.x);

	if (gpHit->check_IsHit())
	{
		gObjectManager.erase("2Dchar");
	}


	// �J�����̍X�V�����i�r���[�ϊ��s��v�Z�j
	gpCamera->Update();

	for (int i = 0; i < MAX_GROUND; i++)
	{
		gpGround1[i]->Update();
	}
}

void Game_Release()
{
	// �e�Ǘ��z��̒��g�����ׂč폜����
	

	// �n�ʂ̗v�f�����ׂč폜����
	for (int i = 0; i < MAX_GROUND; i++)
	{
		gpGround1[i];
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
	for (auto i = gObjectManager.begin();
		i != gObjectManager.end();
		i++)
	{
		delete i->second;
	}
	// �A�z�z��̗v�f��S�폜
	gObjectManager.clear();
}