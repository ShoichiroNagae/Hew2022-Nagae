#pragma once

#pragma comment (lib, "winmm.lib") // timeGetTime�֐��̂���

// �}�N����`
constexpr auto CLASS_NAME = "DX21Smpl";					// �E�C���h�E�N���X�̖��O;
constexpr auto WINDOW_NAME = "HEW 2022 Nagae";				// �E�B���h�E�̖��O;

constexpr auto SCREEN_WIDTH = (1024);	// �E�C���h�E�̕�;
constexpr auto SCREEN_HEIGHT = (576 + 30);	// �E�C���h�E�̍���;

// �\���̂̒�`


// �֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �Q�[���̉�ʕ`��̊֐�
void Game_Init();
void Game_Draw();
void Game_Update();
void Game_Release();

void MeasureDeltaTime();

// �O���[�o���ϐ��̐錾

// ���_�o�b�t�@�p�̕ϐ�
ID3D11Buffer* gpVertexBuffer;

// ���_�������ϐ�
int gNumVertex;

// �e�N�X�`���p�̕ϐ�
ID3D11ShaderResourceView* gpTextureHalOsaka; // HAL OSAKA

// GameObject�N���X�̕ϐ�
GameObject* gpCottage;
GameObject* gpGun;
GameObject* gpPlayer;
GameObject* gpSword;

constexpr auto MAX_GROUND = 10;
GameObject* gpGround[MAX_GROUND][MAX_GROUND];

// ���f���}�l�[�W���[
// �A�z�z�� - �Y���𐮐��ȊO�ɂł���z��

std::map<std::string, ModelData> gModelManager;


// Camera�N���X�̕ϐ�
Camera* gpCamera;

// �f���^�^�C���p�̕ϐ�
DWORD gDeltaTime;

//// �e�N�X�`���z��(���삪�d���Ȃ����ז�����)
 float animTime = 0.000f;
 int animFlame = 0;
//#define AN_DEFOMAX 3
 const wchar_t* animDefo[] =
 { 
	 L"assets/cottage.png",
	 L"assets/ground1.png",
	 L"assets/sword.png"
 };