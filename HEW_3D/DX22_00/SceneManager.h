#pragma once
#include "BaseScene.h"

// シーン管理クラス
class SceneManager {
public:
    enum SCENE {
        TITLE,
        GAME,
        RESULT,
        QUIT,
    };

    static void ChangeScene(SCENE scene);     //シーンを引数によって変える関数
    static void Update();                     //現在のシーンの更新関数
    static void Draw();                     //現在のシーンの描画関数
private:
    static BaseScene* m_pScene;               //現在のシーン
};