#pragma once

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

void Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow);


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
//// �A�z�z�񁁓Y�����𐮐��ȊO�ɂł���z��
//std::map<std::string, ModelData> gModelManager;
//
//// �I�u�W�F�N�g�}�l�[�W���[ �A�z�z��ō쐬
//std::map <std::string, GameObject*> gObjectManager;

// �J�����N���X�̕ϐ�
Camera* gpCamera;

// �f���^�^�C���p�̕ϐ�
DWORD gDeltaTime;