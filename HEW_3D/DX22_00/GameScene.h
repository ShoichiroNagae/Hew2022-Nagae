#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>
#include <random>
#include "ObjModelLoader.h"

// �}�N����`
constexpr auto MAX_GROUND = (10);

// �v���C���[�̎��E���ɓ���X���W�͈̔�
#define ENEMY_MIN_XPOS	(-7.0f);	// �ŏ��l(���[)
#define ENEMY_MAX_XPOS	(7.0f);		// �ő�l(�E�[)
#define ENEMY_SPEED_DEF (0.001f);	// �X�s�[�h�@�����l

class GameScene : public BaseScene
{
public:
	GameScene(void);
	~GameScene(void);

	void Update(void);
	void Draw(void);

	void Init(void);
	void Release(void);

	void CreateConstBuffer(void);

	void CameraInit(Camera* cam);

	// 3D���f�����[�h
	// 1.���[�_�[�� 2.�I�u�W�F�N�g�� 3.obj�t�@�C���� 4.�e�N�X�`���t�@�C����
	void ModelLoad(ObjModelLoader oml,std::string ModelName,
		const char* pObjFileName, const wchar_t* pTexFileName);

	// 2D���f�����[�h
	void ModelLoad(ObjModelLoader oml,std::string ModelName,
		float width, float height, float uvWidth, float uvHeight,const wchar_t* pTexFileName);
	
	// �I�u�W�F�N�g����
	// 1.�I�u�W�F�N�g�� 2.�{�� 3,4,5 x,y,z���W
	void ObjectCreate(std::string objName,
		float mScale, float mx, float my, float mz);

protected:

private:
	Camera* gpCamera; // �J����
	ObjModelLoader loader;	// ���f���̃��[�_�[

	// ���f���}�l�[�W���[
	// ���O�Ŋi�[���� �Ăяo���Ƃ���string�^
	std::map <std::string, ModelData> gModelManager;
	// �I�u�W�F�N�g�}�l�[�W���[
	std::map <std::string, GameObject*> gObjManager;
	// �G�l�~�[�}�l�[�W���[
	std::vector<GameObject*> gEnemyManager;

	// �e�e�}�l�[�W���[
	std::vector<GameObject*> gShotManager;

	//���[�_�[�}�l�[�W���[
	std::vector<ObjModelLoader*> Loader;
	
	DWORD gDeltaTime;

	// �n��
	GameObject* gpGround[MAX_GROUND][MAX_GROUND];

	// �G�������Ő�������֐�
	void CreateEnemy();
};