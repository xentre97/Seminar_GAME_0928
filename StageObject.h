#pragma once
#include "Actor.h"

/// <summary>
/// ステージに配置する破壊可能オブジェクト
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

