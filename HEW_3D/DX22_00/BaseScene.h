#pragma once

#include "ModelData.h"
#include "Direct3D.h"

// ���U���g�ɕK�v�ȃf�[�^���܂Ƃ߂��\����
struct RESULT_DATA
{
    int MAX_SPEED;    // �ō����x        300km/h�ȏ� [A] : 250km/h�ȏ� [B] : 200km/h�ȏ� [C] 150km/h�ȏ� [D]�@����ȉ� [E]
    int AVG_SPEED;    // ���ϑ��x        200km/h�ȏ� [A] : 170km/h�ȏ� [B] : 140km/h�ȏ� [C] 100km/h�ȏ� [D]�@����ȉ� [E]
    int KILL_ENEMY;   // �|�����G�̐�    50�̈ȏ�    [A] : 40�̈ȏ� [B] : 30�̈ȏ� [C] 20�̈ȏ� [D]�@����ȉ� [E]
    int COMBO;        // �R���{��        25�ȏ� [A] : 20�ȏ� [B] : 15�ȏ� [C] 10�ȏ� [D]�@����ȉ� [E]
    int LAND_RANK;    // ���n�����N        1 [A] : 2 [B] : 3 [C] 4 [D]�@5 [E]
};

// �������z�N���X
// �S�ẴN���X�̊�b
class BaseScene 
{
public:
    virtual void Update() = 0;
    virtual void Draw() = 0;

    // �Q�[���V�[�����烊�U���g�V�[���փf�[�^�𑗂邽�ߐÓI�ϐ��Ƃ��Đ錾
    static RESULT_DATA mData;
};