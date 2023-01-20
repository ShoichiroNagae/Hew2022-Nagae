#pragma once

#include "ModelData.h"
#include "Direct3D.h"

// リザルトに必要なデータをまとめた構造体
struct RESULT_DATA
{
    int MAX_SPEED;    // 最高速度        300km/h以上 [A] : 250km/h以上 [B] : 200km/h以上 [C] 150km/h以上 [D]　それ以下 [E]
    int AVG_SPEED;    // 平均速度        200km/h以上 [A] : 170km/h以上 [B] : 140km/h以上 [C] 100km/h以上 [D]　それ以下 [E]
    int KILL_ENEMY;   // 倒した敵の数    50体以上    [A] : 40体以上 [B] : 30体以上 [C] 20体以上 [D]　それ以下 [E]
    int COMBO;        // コンボ数        25以上 [A] : 20以上 [B] : 15以上 [C] 10以上 [D]　それ以下 [E]
    int LAND_RANK;    // 着地ランク        1 [A] : 2 [B] : 3 [C] 4 [D]　5 [E]
};

// 純粋仮想クラス
// 全てのクラスの基礎
class BaseScene 
{
public:
    virtual void Update() = 0;
    virtual void Draw() = 0;

    // ゲームシーンからリザルトシーンへデータを送るため静的変数として宣言
    static RESULT_DATA mData;
};