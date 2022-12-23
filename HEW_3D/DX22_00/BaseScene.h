#pragma once

#include "ModelData.h"
#include "Direct3D.h"

#include <d3d11.h>

// 純粋仮想クラス
// 全てのクラスの基礎
class BaseScene 
{
public:
    virtual void Update() = 0;
    virtual void Draw() = 0;
};