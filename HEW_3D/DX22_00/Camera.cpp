#include "Camera.h"

using namespace DirectX;

Camera* Camera::mMainCamera;

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

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return mViewMatrix;
}

void Camera::Update()
{
	// ビュー変換行列を計算
	XMVECTOR eye = XMLoadFloat3(&mEye);
	XMVECTOR focus = XMLoadFloat3(&mFocus);
	XMVECTOR Up = XMLoadFloat3(&mUp);
	mViewMatrix = XMMatrixLookAtLH(
		eye, focus, Up);
}
