#pragma once
#include "Actor.h"

/// <summary>
/// ステージを構成するオブジェクト
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

//class HardObj :
//    public StageObject
//{
//public:
//    HardObj(class Sequence* sequence)
//};