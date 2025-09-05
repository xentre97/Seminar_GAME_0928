#pragma once
#include "UIScreen.h"
#include <raylib.h>

/// <summary>
/// HUD�̕\����S������N���X
/// </summary>
class HUD :
    public UIScreen
{
public:
    HUD(class Sequence* sequence);
    
    void update() override;
    void draw() override;

private:
    Rectangle mHpbar;       // hp�o�[�̔w�i
    Rectangle mCurrentHp;   // ���݂�hp��\������
};

