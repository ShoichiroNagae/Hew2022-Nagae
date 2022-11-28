#include "SceneManager.h"
#include "Scene_Title.h"
#include "Scene_Game.h"
#include "Scene_Clear.h"

// static�ϐ��̎��̉�
Scene_Base* SceneManager::pScene = nullptr;

void SceneManager::ChangeScene(SCENE scene)
{
	// ���݂̃V�[���ϐ��ɓ��e�������Ă���Ȃ�
	if (pScene != nullptr) {
		delete pScene;
	}

	//�X�ԋ���
	/// <summary>
	/// �V�[���̑J��
	/// </summary>
	/// <param name="scene"> �V�[���̎w�� (enum�Œ�`)</param>
	switch (scene){
	case SceneManager::SCENE::TITLE:
		pScene = new Scene_Title();
		break;
	case SceneManager::SCENE::GAME:
		pScene = new Scene_Game();
		break;
	case SceneManager::SCENE::CLEAR:
		pScene = new Scene_Clear();
		break;
	default:
		break;
	}
}

void SceneManager::Update()
{
	pScene->Update();
}

void SceneManager::Draw()
{
	pScene->Update();
}
