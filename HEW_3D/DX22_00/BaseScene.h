#pragma once

#include "ModelData.h"
#include "Direct3D.h"

#include <d3d11.h>

// �������z�N���X
// �S�ẴN���X�̊�b
class BaseScene 
{
public:
    virtual void Update() = 0;
    virtual void Draw() = 0;
};