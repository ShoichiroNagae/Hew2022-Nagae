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
	// テクスチャ
	ID3D11ShaderResourceView* gpTextureTitle;
	ID3D11ShaderResourceView* gpTextureSpaceKey;

	// オブジェクト
	StaticObject* gpTitleLogo;
	StaticObject* gpSpaceKey;
};

