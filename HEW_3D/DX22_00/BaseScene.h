#pragma once

// 純粋仮想クラス
// 全てのクラスの基礎
class BaseScene 
{
public:
    virtual void Update() = 0;
    virtual void Draw() = 0;
};