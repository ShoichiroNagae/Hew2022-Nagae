#include "SceneManager.h"

// �S�V�[���w�b�_�[include
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"

BaseScene* SceneManager::m_pScene = nullptr;       //�n�߂̓V�[������ɂ��Ă�����

void SceneManager::ChangeScene(SCENE scene) {

    // ���݂̃V�[����Delete
    if (m_pScene != nullptr) {
        delete m_pScene;
    }

    // �����Ƃ��ēn���ꂽ�V�[���Ɉړ�
    switch (scene) {
    case SCENE::TITLE:
        m_pScene = new TitleScene();      //�^�C�g���V�[�������݂̃V�[���ɂ���
        break;
    case SCENE::GAME:
        m_pScene = new GameScene();       //�Q�[���V�[�������݂̃V�[���ɂ���
        break;
    case SCENE::RESULT:
        m_pScene = new ResultScene();     //���U���g�V�[�������݂̃V�[���ɂ���
        break;
    default:
        break;
    }

}

void SceneManager::Update() {
    m_pScene->Update();         //���݂̃V�[���̍X�V�֐�
}

void SceneManager::Draw() {
    m_pScene->Draw();           //���݂̃V�[���̕`��֐�
}