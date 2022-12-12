#pragma once
#include "BaseScene.h"
#include "StaticObject.h"

class TitleScene : public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	void Update();
	void Draw();
	void LoadTexture(const wchar_t* fileName,
		ID3D11ShaderResourceView** outTexture);

	DIRECT3D* d3d;

private:
	// �e�N�X�`��
	ID3D11ShaderResourceView* gpTextureTitle;
	ID3D11ShaderResourceView* gpTextureSpaceKey;

	// �I�u�W�F�N�g
	StaticObject* gpTitleLogo;
	StaticObject* gpSpaceKey;
};

