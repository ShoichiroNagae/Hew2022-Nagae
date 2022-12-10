//
//
//
//
//
// //
//
// //
//
//

#undef UNICODE  // Unicode�ł͂Ȃ��A�}���`�o�C�g�������g��

#include <Windows.h>
#include "Direct3d.h"
#include "input.h"
#include <DirectXMath.h>
#include "WICTextureLoader.h"
#include "ObjModelLoader.h"
#include "Model.h"
#include "Camera.h"
#include "GameObject.h"
#include <map>
#include <string>
#include "Winmain.h"

extern ID3D11Buffer* gpConstBuffer;
using namespace DirectX;

// WinMain�֐������
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wc{}; // WND = Window

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

	HWND hWnd;					// H=Handle=�|�C���^ WND=Window
	hWnd = CreateWindowEx(0,	// �g���E�B���h�E�X�^�C��
		CLASS_NAME,				// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,			// �E�B���h�E�̖��O
		WS_OVERLAPPED
		| WS_SYSMENU
		| WS_MINIMIZEBOX,		// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,			// �E�B���h�E�̍���w���W
		CW_USEDEFAULT,			// �E�B���h�E�̍���x���W 
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
			MeasureDeltaTime();	//�f���^�^�C���v��
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

// �f���^�^�C���̌v��
void MeasureDeltaTime()
{
	static DWORD lastTime = timeGetTime(); // �O��v�����̎���
	timeBeginPeriod(1); // ���x���P�~���b�ɏグ��
	DWORD nowTime = timeGetTime(); // ���݂̎���
	timeEndPeriod(1); // ���x�����ɖ߂�
	gDeltaTime = nowTime - lastTime; // �������f���^�^�C��
	lastTime = nowTime; // �O��v�����ԂƂ��ĕۑ�
}

void Game_Init()
{
	// �萔�o�b�t�@�쐬
	// �R���X�^���g�o�b�t�@�Ƃ��č쐬���邽�߂̏��ݒ�
	D3D11_BUFFER_DESC contstat_buffer_desc{};
	contstat_buffer_desc.ByteWidth = 4 * 4 * 4 * 4;	// �o�b�t�@�̃T�C�Y�i4x4�s��x4�j
	contstat_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;		// �g�p���@
	contstat_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �o�b�t�@�̎��(�R���X�^���g�o�b�t�@)
	contstat_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPU�A�N�Z�X�ݒ�
	contstat_buffer_desc.MiscFlags = 0;				// ���̑��̃I�v�V����
	contstat_buffer_desc.StructureByteStride = 0;			// �\���̃T�C�Y(�s����g���������0�ł����삷�邱�Ƃ����؂���)

	Direct3D_Get()->device->CreateBuffer(&contstat_buffer_desc,
		nullptr, &gpConstBuffer);

	// �J�����̍쐬
	gpCamera = new Camera();

	// �����l�ݒ�
	// ���ӁFeye��focus���������ƃ_��
	// ���ӁFup��xyz�S�ă[�����ƃ_��
	gpCamera->SetEye(XMFLOAT3(0.0f, 0.0f, -2.0f));
	gpCamera->SetFocus(XMFLOAT3(0.0f, 0.0f, 0.0f));
	gpCamera->SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));

	// ���[�_�[�Ɉڍs
	// �R�e�[�W���f���ǂݍ���
	ObjModelLoader loader;
	gModelManager["cottage"] = loader.Load(
		"assets/cottage.obj", L"assets/cottage.png");

	// �R�e�[�W�pModel�I�u�W�F�N�g����
	gpCottage = new GameObject();
	Model* pCottageModel = gpCottage->GetModel();
	pCottageModel->SetModelData(gModelManager["cottage"]); // 3D�f�[�^���Z�b�g����
	pCottageModel->SetScale(0.0005f);
	pCottageModel->mPos.z = 2.0f;
	pCottageModel->mCamera = gpCamera;

	// �e���f���ǂݍ���
	ObjModelLoader loader2;
	gModelManager["gun"] = loader2.Load(
		"assets/gun.obj", L"assets/gun.png");

	// �e�pModel�I�u�W�F�N�g����
	gpGun = new GameObject();
	Model* pGunModel = gpGun->GetModel();
	pGunModel->SetModelData(gModelManager["gun"]);
	pGunModel->SetScale(1.5f);
	pGunModel->mPos.z = 1.0f;
	pGunModel->mRotate.y = 90.0f;
	pGunModel->mCamera = gpCamera;

	// �n�ʃ��f���ǂݍ���
	ObjModelLoader loader3;
	gModelManager["ground1"] = loader3.Load(
		"assets/ground1.obj", L"assets/ground1.jpg"
	);

	// �n�ʐ���
	// 2�dFor���[�v�Ő���
	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++){
			gpGround[i][j] = new GameObject();
			Model* pGroundModel = gpGround[i][j]->GetModel();
			pGroundModel->SetModelData(gModelManager["ground1"]);
			pGroundModel->SetScale(1.0f);
			pGroundModel->mPos.x = -10.0f + 2.0f * i;
			pGroundModel->mPos.z = -10.0f + 2.0f * j;
			pGroundModel->mPos.y = -1.0f;
			pGroundModel->mCamera = gpCamera;
		}
	}

	// �\�[�h���f���ǂݍ���
	ObjModelLoader loader4;
	gModelManager["Sword"] = loader4.Load(
		"assets/sword.obj", L"assets/sword.png"
	);

	// �\�[�h���f������
	gpSword = new GameObject();
	Model* pSwordModel = gpSword->GetModel();
	pSwordModel->SetModelData(gModelManager["Sword"]);
	pSwordModel->SetScale(0.03f);
	pSwordModel->mPos.z = 1.0f;
	pSwordModel->mRotate.y = 90.0f;
	pSwordModel->mCamera = gpCamera;

	// ���[�_�[�Ɉڍs

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

	// �Q�[���I�u�W�F�N�g��`��
	gpCottage->Draw();
	gpSword->Draw();

	for (int i = 0; i < MAX_GROUND; i++){
		for (int j = 0; j < MAX_GROUND; j++) {
			gpGround[i][j]->Draw();
		}
	}

	// �_�u���o�b�t�@�̐؂�ւ�
	d3d->swapChain->Present(0, 0);

}

void Game_Update()
{
	// �f���^�^�C�����z��O�̒l�ɂȂ����ꍇ�̏���
	if (gDeltaTime >= 100){
		gDeltaTime = 0;
	}
	if (gDeltaTime <= 0){
		gDeltaTime = 1;
	}

	// �J�����ړ��ϐ�
	static float angle = 0.0f; // ��]�p�x
	static float zoom = 3.0f;  // �Y�[��

	// �@�J�����̈ʒu���L�[����ňړ�����
	if (Input_GetKeyDown(VK_RIGHT))
	{
		angle += 0.04f * gDeltaTime;
	}
	if (Input_GetKeyDown(VK_LEFT))
	{
		angle -= 0.04f * gDeltaTime;
	}

	// �Y�[������
	if (Input_GetKeyDown(VK_UP))
		zoom -= 0.01f * gDeltaTime;
	if (Input_GetKeyDown(VK_DOWN))
		zoom += 0.01f * gDeltaTime;

	// �A�J�����̒����_�𒆐S�ɃJ��������]����

	// �J�����ʒuX�@���@sinf(�p�x���W�A��)
	// �J�����ʒuZ�@���@cosf(�p�x���W�A��)
	// ���_�𒆐S�ɔ��a1.0f�̉~����̓_�����߂�
	Model* pCottageModel = gpCottage->GetModel();
	float radian = XMConvertToRadians(angle);
	gpCamera->mEye.x =
		sinf(radian) * zoom + pCottageModel->mPos.x;
	gpCamera->mEye.z =
		cosf(radian) * zoom + pCottageModel->mPos.z;
	gpCamera->mEye.y = 2.0f;

	// �J���������_���R�e�[�W�̈ʒu�ɂ���
	gpCamera->SetFocus(pCottageModel->mPos);

	// �L�����N�^�[�ړ�
	// �L�����N�^�[�������Ă�������ɑO�i����
	// �����ύX��AD�L�[�@�O�i��W�L�[
	// �u�O�����x�N�g���v���v�Z����
	// �ړ����x��W�L�[�Ō��܂�
	
	//gpGun->mSpeed = 0.0f;
	//if (Input_GetKeyDown('W'))
	//	gpGun->mSpeed = 0.001f;
	//if (Input_GetKeyDown('S'))
	//	gpGun->mSpeed = -0.001f;

	gpSword->mSpeed = 0.0f;
	if (Input_GetKeyDown('W'))
		gpSword->mSpeed = 0.001f;
	if (Input_GetKeyDown('S'))
		gpSword->mSpeed = -0.001f;

	// �L�����N�^�[�̕����]��
	//Model* pGunModel = gpGun->GetModel();
	//if (Input_GetKeyDown('A'))
	//	pGunModel->mRotate.y -= 0.04f * gDeltaTime;
	//if (Input_GetKeyDown('D'))
	//	pGunModel->mRotate.y += 0.04f * gDeltaTime;

	Model* pSwordModel = gpSword->GetModel();
	if (Input_GetKeyDown('A'))
		pSwordModel->mRotate.y -= 0.04 * gDeltaTime;
	if (Input_GetKeyDown('D'))
		pSwordModel->mRotate.y += 0.04 * gDeltaTime;

	gpCottage->Update();
	/*gpGun->Update();*/
	gpSword->Update();

	// �J�����Ǐ]����
	// 1.����L�����̑O�����x�N�g��������Ă���

	/*XMFLOAT3 forwardVec = gpGun->GetForwardVector();*/
	XMFLOAT3 forwardVec = gpSword->GetForwardVector();

	// 2.���̑O�����x�N�g���𔽓]���āC
	// �������x�N�g�������
	XMFLOAT3 backVec{};
	backVec.x = -forwardVec.x;
	backVec.z = -forwardVec.z;
	backVec.y = -forwardVec.y;

	// 3.�������x�N�g�����g���āC�L�����̌��ɃJ�����̏œ_�𓖂Ă�
	XMFLOAT3 camEye{};
	/*camEye.x = pGunModel->mPos.x + backVec.x * 2.0f;
	camEye.z = pGunModel->mPos.z + backVec.z * 2.0f;
	camEye.y = pGunModel->mPos.y + backVec.y * 2.0f + 1.0f;*/

	camEye.x = pSwordModel->mPos.x + backVec.x * 2.0f;
	camEye.z = pSwordModel->mPos.z + backVec.z * 2.0f;
	camEye.y = pSwordModel->mPos.y + backVec.y * 2.0f + 1.0f;

	// �ɂ₩�J���������
	// 1�t���[���O�̃J����1��ۑ�����ϐ�
	static XMFLOAT3 lastCamEye;
	float blendFactor = 0.995f;
	camEye.x = lastCamEye.x * blendFactor
		+ camEye.x * (1.0f - blendFactor);
	camEye.y = lastCamEye.y * blendFactor
		+ camEye.y * (1.0f - blendFactor);
	camEye.z= lastCamEye.z * blendFactor
		+ camEye.z * (1.0f - blendFactor);
	gpCamera->SetEye(camEye);
	lastCamEye = camEye;

	// �J���������_��ݒ�
	// ����L�����̏����O�𒍎��_�ɂ���
	XMFLOAT3 camFocus{};
	camFocus.x = pSwordModel->mPos.x + forwardVec.x;
	camFocus.z = pSwordModel->mPos.z + forwardVec.z;
	camFocus.y = pSwordModel->mPos.y + forwardVec.y;

	gpCamera->SetFocus(camFocus);

	for (int i = 0; i < MAX_GROUND; i++) {
		for (int j = 0; j < MAX_GROUND; j++) {
			gpGround[i][j]->Update();
		}
	}
}

void Game_Release()
{
	delete gpGun;
	delete gpCottage;
	delete gpSword;

	for (int i = 0; i < MAX_GROUND; i++) {
		for (int j = 0; j < MAX_GROUND; j++) {
			delete gpGround[i][j];
		}
	}

	//���f���}�l�[�W���[���Ǘ����郂�f����S�J������
	// �擪�̃C�e���[�^��Ԃ�
	for (auto obj = gModelManager.begin();
		// �Ō�̃C�e���[�^�ƈ�v����H�H
		obj != gModelManager.end();
		obj++)
	{
		// first �� �Y��
		// second �� �i�[����Ă���v�f

		COM_SAFE_RELEASE(obj->second.mSRV);
		COM_SAFE_RELEASE(obj->second.mVertexBuffer);
	};

	// �A�z�z��̗v�f��S�폜
	gModelManager.clear();

	COM_SAFE_RELEASE(gpConstBuffer);
}


