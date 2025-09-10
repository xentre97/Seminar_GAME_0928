#pragma once
#include "Actor.h"

/// <summary>
/// �X�e�[�W�ɔz�u����j��\�I�u�W�F�N�g
/// </summary>
class StageObject :
    public Actor
{
public:
    StageObject(class Sequence* sequence);
    ~StageObject();

    void update() override;
    void computeRectangle() override;
    class HpComponent* getHpComp() { return mHpComp; }

private:
    class SpriteComponent* mSpriteComp;
    class HpComponent* mHpComp;
};

