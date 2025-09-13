#pragma once
#include "Component.h"
#include "AttackInfo.h"

/// <summary>
/// 攻撃を行う(HPを減らす,ノックバック付与,効果音鳴らす)機能を付与するcomponent
/// 外部からAttackInfoを受け取り、それに基づいた処理をする
/// </summary>
class AttackComponent :
    public Component
{
public:
    AttackComponent(class Actor* owner);

    void update() override;
    // 攻撃する際に呼び出す,攻撃中にも呼べる
    void startAttack(AttackInfo* info);

private:
    void processAttackEnemy();
    void processAttackPlayer();
    
    float mTimer;
    // 現在の攻撃情報
    AttackInfo* mCurInfo;
    bool mActive;
};

