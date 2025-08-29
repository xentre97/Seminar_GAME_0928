#pragma once
#include "Component.h"

/// <summary>
/// アクターにHpを持たせる
/// Component化したのは、敵・プレイヤー・破壊できるオブジェクト等にアタッチすることを想定したため
/// </summary>
class HpComponent :
    public Component
{
public:
    HpComponent(class Actor* owner, float maxHp = 1.0f);

    void TakeDamage(float damage);
    void Recover(float recover);

    // 0以下で
    bool IsKilled() const { return mCurHp <= 0.0f; }

    // Getter,Setter
    float GetCurHp() const { return mCurHp; }
    void SetMaxHp(float maxHp) { mMaxHp = maxHp, mCurHp = maxHp; }
    // 便利系(SetterGetterでもできるけど...という奴)
    float GetHpPercentage() const { return mCurHp / mMaxHp; }
    // 最大Hp増やす
    void AddMaxHp(float add);

private:
    float mCurHp;   // 現在Hp
    float mMaxHp;   // 最大Hp こっちはActor内で持たせてもいいと思います(作ってみて次第)
};

