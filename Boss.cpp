//===============================================
// Boss.cpp
//===============================================
#include "Boss.h"
#include "GamePlay.h"
#include "AnimSpriteComponent.h"
#include "AttackComponent.h"
#include "GuardComponent.h"
#include "HpComponent.h"
#include "LobProjectile.h"
#include "BeamActor.h"
#include "MinionActor.h"
#include "BossMove.h"

// 調整用の定数（実機プレイで様子を見ながら調整してOK）
static const float kBossHpMax           = 400.0f; // HP
static const float kBossGuardMax        = 200.0f; // ガード
static const float kGuardBreakIFrames   = 0.5f;   // ガードブレイク直後のガード無敵
static const float kHpLeakWhileGuard    = 0.10f;  // ガード中、HPへ 10% 通す

static const float kCD_MeleeLight       = 2.0f;
static const float kCD_MeleeHeavy       = 4.5f;
static const float kCD_Lob              = 3.5f;
static const float kCD_Beam             = 6.0f;

static const float kSummonInterval      = 8.0f;   // 召喚間隔
static const int   kSummonCount         = 2;      // 1回に何体
static const float kMinionLifeSeconds   = 6.0f;   // ミニオン寿命
static const float kMinionHealAmount    = 12.0f;  // 自滅時の回復量

static const float kMeleeLightDamage    = 15.0f;
static const float kMeleeHeavyDamage    = 35.0f;
static const float kLobDamage           = 20.0f;  // LobProjectile側で適用
static const float kBeamDamage          = 18.0f;  // BeamActor側で適用

Boss::Boss(GamePlay* sequence)
    : EnemyActor(sequence)
{
    // 基本ステータス
    mHpComp->SetMaxHp(kBossHpMax);
    mGuard = new GuardComponent(this, kBossGuardMax, kGuardBreakIFrames);
    // 移動コンポーネント（EnemyMove 踏襲）
    mMove = new BossMove(this);

    // ステージ最奥側に配置（座標はプロジェクトに合わせて調整）
    setPosition({ 1400.0f, 360.0f });
    mForward = -1; // 左向き
}

void Boss::update() {
    // まずは EnemyActor 側（アニメ等）を更新
    EnemyActor::update();

    // クールダウンを進める
    const float dt = GetFrameTime();
    if (mMeleeLightCd > 0) mMeleeLightCd -= dt;
    if (mMeleeHeavyCd > 0) mMeleeHeavyCd -= dt;
    if (mLobCd        > 0) mLobCd        -= dt;
    if (mBeamCd       > 0) mBeamCd       -= dt;

    // 召喚とガード再生条件のチェック
    trySummon(dt);
    tryGuardRecharge();

    // サンプルAI：クールダウン到達で順番に発動（実用時は距離/ライン等で切り替え推奨）
    if (mMeleeLightCd <= 0) { doMeleeLight();   mMeleeLightCd = kCD_MeleeLight; }
    if (mMeleeHeavyCd <= 0) { doMeleeHeavy();   mMeleeHeavyCd = kCD_MeleeHeavy; }
    if (mLobCd        <= 0) { doLobProjectile(); mLobCd       = kCD_Lob;        }
    if (mBeamCd       <= 0) { doBeam();         mBeamCd       = kCD_Beam;       }
}

void Boss::computeRectangle() {
    // とりあえず 128x128。スプライトの見た目に合わせて調整推奨。
    mRectangle.x      = mPosition.x - 64.0f;
    mRectangle.y      = mPosition.y - 64.0f;
    mRectangle.width  = 128.0f;
    mRectangle.height = 128.0f;
}

void Boss::ApplyDamage(float dmg, DamageTag tag) {
    // ガードが残っている間は、ガードに吸収 + HPへは 10% だけ通す
    if (mGuard->HasGuard()) {
        mGuard->TakeGuardDamage(dmg, tag);
        mHpComp->TakeDamage(dmg * kHpLeakWhileGuard);
    } else {
        // ガードが無いときは通常通り HP へ
        mHpComp->TakeDamage(dmg);
    }
}

void Boss::Heal(float hp) {
    // 召喚ミニオン自滅時に呼ばれる
    mHpComp->Recover(hp);
}

void Boss::tryGuardRecharge() {
    // HP の割合を見て、50%/25% 到達で一度だけガード全回復
    const float ratio = mHpComp->GetHpRatio();
    if (!mDidHalfRecharge && ratio <= 0.5f) {
        mGuard->RechargeFull();
        mDidHalfRecharge = true;
    }
    if (!mDidQuarterRecharge && ratio <= 0.25f) {
        mGuard->RechargeFull();
        mDidQuarterRecharge = true;
    }
}

void Boss::doMeleeLight() {
    // 近接・軽攻撃：短時間の近距離矩形
    auto* atk = new AttackComponent(this);
    AttackInfo info{};                 // ★ AttackInfo.h に DamageTag フィールド追加が必要
    info.damage     = kMeleeLightDamage;
    info.duration   = 0.15f;
    info.colRect    = { mRectangle.x - 10.0f, mRectangle.y + 10.0f, mRectangle.width + 20.0f, mRectangle.height - 20.0f };
    info.knockBack  = 150.0f;
    info.targetType = Actor::Eplayer;
    info.tag        = DamageTag::MeleeLight;
    atk->startAttack(&info);
}

void Boss::doMeleeHeavy() {
    // 近接・重攻撃：長め・強め
    auto* atk = new AttackComponent(this);
    AttackInfo info{};
    info.damage     = kMeleeHeavyDamage;
    info.duration   = 0.35f;
    info.colRect    = { mRectangle.x - 20.0f, mRectangle.y, mRectangle.width + 40.0f, mRectangle.height };
    info.knockBack  = 300.0f;
    info.targetType = Actor::Eplayer;
    info.tag        = DamageTag::MeleeHeavy;
    atk->startAttack(&info);
}

void Boss::doLobProjectile() {
    // 放物線投擲：弾Actorに挙動を委譲
    auto* gp = static_cast<GamePlay*>(getSequence());
    new LobProjectileActor(gp, { mPosition.x, mPosition.y - 40.0f }, mForward);
}

void Boss::doBeam() {
    // 直線ビーム：瞬間的に長いヒットボックス
    auto* gp = static_cast<GamePlay*>(getSequence());
    new BeamActor(gp, { mPosition.x + mForward * 32.0f, mPosition.y - 20.0f }, mForward);
}

void Boss::trySummon(float dt) {
    // 一定間隔ごとにミニオンを召喚。各ミニオンは寿命で自滅し、ボスを回復。
    mSummonTimer += dt;
    if (mSummonTimer >= kSummonInterval) {
        mSummonTimer = 0.0f;
        auto* gp = static_cast<GamePlay*>(getSequence());
        for (int i = 0; i < kSummonCount; ++i) {
            auto* m = new MinionActor(gp, this, kMinionLifeSeconds, kMinionHealAmount);
            m->setPosition({ mPosition.x - 120.0f - 40.0f * i, mPosition.y });
        }
    }
}
