#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>
#include "ObjModelLoader.h"

// �}�N����`
constexpr auto MAX_GROUND = (10);

class GameScene : public BaseScene
{
public:
	GameScene(void);
	~GameScene(void);

	void Update(void);
	void Draw(void);

	void Init(void);
	void Release(void);

	void CreateConstBudder(void);

	void CameraInit(Camera* cam);

	// 3D���f�����[�h
	// 1.���[�_�[�� 2.�I�u�W�F�N�g�� 3.obj�t�@�C���� 4.�e�N�X�`���t�@�C����
	void ModelLoad(ObjModelLoader oml,std::string ModelName,
		const char* pObjFileName, const wchar_t* pTexFileName);

protected:

private:
	Camera* gpCamera; // �J����
	ObjModelLoader loader;	// ���f���̃��[�_�[

	// ���f���}�l�[�W���[
	// ���O�Ŋi�[���� �Ăяo���Ƃ���string�^
	std::map <std::string, ModelData> gModelManager;
	// �I�u�W�F�N�g�}�l�[�W���[
	std::map <std::string, GameObject*> gObjManager;
	// �e�e�}�l�[�W���[
	std::vector<GameObject*> gShotManager;

	//���[�_�[�}�l�[�W���[
	std::vector<ObjModelLoader*> Loader;
	
	DWORD gDeltaTime;

	GameObject* gpGround[MAX_GROUND][MAX_GROUND];
};