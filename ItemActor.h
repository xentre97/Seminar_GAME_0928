#pragma once
#include "Actor.h"

/// <summary>
/// アイテムの基底クラス
/// プレイヤーに当たると影響を及ぼす
/// </summary>
class ItemActor :
    public Actor
{
public:
    ItemActor(class Sequence* sequence);
    virtual ~ItemActor() {};

    void update() override;
    void computeRectangle() override;
    // プレイヤーが触れた時に
    virtual void onAcquired() = 0;

protected:
    class SpriteComponent* mSpriteComp;
};

/// <summary>
/// 回復アイテム
/// </summary>
class HealingItem
    : public ItemActor
{
public:
    HealingItem(class Sequence* sequence);
    void onAcquired() override;
private:
    float mHealAmount;
};

/// <summary>
/// 移動速度上昇アイテム
/// </summary>
//class SpeedUpItem
//    : ItemActor
//{
//public:
//
//
//};