#pragma once

#include "Direct3D.h"
#include <DirectXMath.h>

#include "GameObject.h"
#include <map>
#include <vector>

// �}�N����`
constexpr auto CLASS_NAME = "DX21Smpl";					// �E�C���h�E�N���X�̖��O
constexpr auto WINDOW_NAME = "�X�P���g���v���O����";	// �E�B���h�E�̖��O

constexpr auto SCREEN_WIDTH = (1024);					// �E�C���h�E�̕�
constexpr auto SCREEN_HEIGHT = (576 + 30);				// �E�C���h�E�̍���

// �֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �Q�[��������
void Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow);

// ���_�o�b�t�@�p�̕ϐ�
ID3D11Buffer* gpVertexBuffer;

// ���_�������ϐ�
int gNumVertex;

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

// �f���^�^�C���p�̕ϐ�
DWORD gDeltaTime;