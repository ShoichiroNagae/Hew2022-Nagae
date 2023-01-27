#pragma once
#include "BaseScene.h"

// �V�[���Ǘ��N���X
class SceneManager {
public:
    enum SCENE {
        TITLE,
        GAME,
        RESULT,
        QUIT,
    };

    static void ChangeScene(SCENE scene);     //�V�[���������ɂ���ĕς���֐�
    static void Update();                     //���݂̃V�[���̍X�V�֐�
    static void Draw();                     //���݂̃V�[���̕`��֐�
private:
    static BaseScene* m_pScene;               //���݂̃V�[��
};