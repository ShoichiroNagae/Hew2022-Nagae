#include "SceneManager.h"

// 全シーンヘッダーinclude
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"

BaseScene* SceneManager::m_pScene = nullptr;       //始めはシーンを空にしておこう

void SceneManager::ChangeScene(SCENE scene) {

    // 現在のシーンをDelete
    if (m_pScene != nullptr) {
        delete m_pScene;
    }

    // 引数として渡されたシーンに移動
    switch (scene) {
    case SCENE::TITLE:
        m_pScene = new TitleScene();      //タイトルシーンを現在のシーンにする
        break;
    case SCENE::GAME:
        m_pScene = new GameScene();       //ゲームシーンを現在のシーンにする
        break;
    case SCENE::RESULT:
        m_pScene = new ResultScene();     //リザルトシーンを現在のシーンにする
        break;
    default:
        break;
    }

}

void SceneManager::Update() {
    m_pScene->Update();         //現在のシーンの更新関数
}

void SceneManager::Draw() {
    m_pScene->Draw();           //現在のシーンの描画関数
}