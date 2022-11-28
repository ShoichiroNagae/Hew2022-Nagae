#include "SceneManager.h"
#include "Scene_Title.h"
#include "Scene_Game.h"
#include "Scene_Clear.h"

// static変数の実体化
Scene_Base* SceneManager::pScene = nullptr;

void SceneManager::ChangeScene(SCENE scene)
{
	// 現在のシーン変数に内容が入っているなら
	if (pScene != nullptr) {
		delete pScene;
	}

	//９番強い
	/// <summary>
	/// シーンの遷移
	/// </summary>
	/// <param name="scene"> シーンの指定 (enumで定義)</param>
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
