#pragma once
//===============================================
// Boss.h
//  最奥に鎮座するボス本体。
//  ・ガードゲージ（GuardComponent）
//  ・近接：軽/重／遠距離：投擲/ビーム の4攻撃
//  ・時間経過で雑魚召喚 → 自滅でボス回復
//  ・HPが 1/2・1/4 を下回るとガード全回復
//===============================================
#include "EnemyActor.h"
#include "DamageTags.h"

class GuardComponent;
class HpComponent;
class GamePlay;

class Boss : public EnemyActor {
public:
    explicit Boss(GamePlay* sequence);

    void update() override;           // 思考・攻撃・召喚など
    void computeRectangle() override; // 当たり矩形（スプライトに合わせて調整推奨）

    // ダメージ適用（AttackComponent/ExplosionActor から呼ばれる想定）
    void ApplyDamage(float dmg, DamageTag tag);

    // 召喚雑魚の自滅で呼ぶ回復
    void Heal(float hp);

    GuardComponent* getGuard() const { return mGuard; }
    HpComponent*    getHp()    const { return mHpComp; }

private:
    // 攻撃ハンドラ（実際の発生体は AttackComponent/Projectile など）
    void doMeleeLight();
    void doMeleeHeavy();
    void doLobProjectile();
    void doBeam();

    // 召喚とガード再生
    void trySummon(float dt);
    void tryGuardRecharge();

private:
    GuardComponent* mGuard = nullptr; // ボスのガード
    class BossMove* mMove = nullptr; // ボスの移動

    // 攻撃クールダウン（0以下で使用可能）
    float mMeleeLightCd = 0.0f;
    float mMeleeHeavyCd = 0.0f;
    float mLobCd        = 0.0f;
    float mBeamCd       = 0.0f;

    // 召喚タイマ
    float mSummonTimer    = 0.0f;
    float mSummonInterval = 8.0f; // 8秒ごとに召喚

    // ガード再生が既に実行済みかのフラグ
    bool mDidHalfRecharge    = false; // HP<=50% で一度だけ
    bool mDidQuarterRecharge = false; // HP<=25% で一度だけ
};
