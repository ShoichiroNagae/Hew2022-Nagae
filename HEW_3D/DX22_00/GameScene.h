#pragma once
#include "BaseScene.h"
#include "BackCamera.h"
#include <map>
#include <string>
#include <vector>
#include <random>
#include "ObjModelLoader.h"

// �}�N����`
constexpr auto MAX_GROUND = (100);

// �v���C���[�̎��E���ɓ���X���W�͈̔�
#define ENEMY_MIN_XPOS	(-5.0f);	// �ŏ��l(���[)
#define ENEMY_MAX_XPOS	(3.0f);		// �ő�l(�E�[)
#define ENEMY_SPEED_DEF (0.001f);	// �X�s�[�h�@�����l

#define GROUND_POS	(400.0f)

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
	
	// �r���{�[�h�A�j���[�V�����֌W�n�p���[�_�[�@�@�쐬::��ˏ�
	void ModelLoad(ObjModelLoader oml, std::string ModelName,
		DirectX::XMFLOAT4 Set_whuv, const wchar_t* pTexFileName);

	// �I�u�W�F�N�g����
	// 1.�I�u�W�F�N�g�� 2.�{�� 3,4,5 x,y,z���W
	void ObjectCreate(std::string objName,
		float mScale, float mx, float my, float mz);

protected:

private:
	Camera*				gpCamera;			// �J����
	ObjModelLoader		loader;				// ���f���̃��[�_�[

	int					frameCount;			// �t���[���J�E���g
	int					frameCountChange;	// ���U���g�V�[���֑J�ڂ��鎞�̃J�E���g
	int					nowCombo;			// ���݂̃R���{��
	int					maxCombo;			// �ő�R���{��
	bool				playerLanded;		// �v���C���[���n�ʂɂ������ǂ���

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
	std::vector<GameObject*> gGround;

	// �G�������Ő�������֐�
	void CreateEnemy();
	// �G�������Ă����������肷��֐�
	bool CheckEnemy(GameObject* _enemy);
	// �v���C���[�̈ړ��͈͂𐧌�����
	void MoveLimit();
	// �U���A�j���[�V������
	int SetAnimState = 0;
};