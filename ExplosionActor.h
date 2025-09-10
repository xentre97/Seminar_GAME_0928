#pragma once
#include "Actor.h"

/// <summary>
/// 爆発アクター,敵が倒された時などに生成する エフェクトみたいなイメージ
/// 以下Actorにした理由
/// 当たり判定用矩形やテクスチャを独立して扱うから
/// また敵以外にも当たることから,独立して扱いたいから
/// 分業しやすそうと思ったから
/// </summary>
class ExplosionActor :
    public Actor
{
public:
    ExplosionActor(class GamePlay* sequence);

    void update() override;

private:
    void computeRectangle() override;
    static struct Animation mAnim;
    class AnimSpriteComponent* mAnimsc;
    class GamePlay* mGamePlay;
    float mDamageAmt;
    float mTimer;
    float mActiveTime;
};

