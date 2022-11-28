#include "Camera.h"

using namespace DirectX;

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	// XMFLOAT3��XMVECTOR�֕ϊ�
	XMVECTOR eye = XMLoadFloat3(&mEye);
	XMVECTOR focus = XMLoadFloat3(&mFocus);
	XMVECTOR up = XMLoadFloat3(&mUp);
	// �r���[�ϊ��s��쐬
	XMMATRIX mxView = XMMatrixLookAtLH(
		eye, focus, up);

	return mxView;
}

void Camera::SetEye(DirectX::XMFLOAT3 newEye)
{
	mEye = newEye;
}

void Camera::SetFocus(DirectX::XMFLOAT3 newFocus)
{
	mFocus = newFocus;
}

void Camera::SetUp(DirectX::XMFLOAT3 newUp)
{
	mUp = newUp;
}
