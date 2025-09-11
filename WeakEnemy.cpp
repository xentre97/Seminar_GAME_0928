#include "WeakEnemy.h"
#include "GamePlay.h"
#include "Stage.h"

#include "EnemyMove.h"
#include "AnimSpriteComponent.h"
#include "HpComponent.h"
// weapon
#include "SwordComponent.h"
#include "ArrowComponent.h"
#include "WeaponActor.h"
#include "PlayerActor.h"

#include "ExplosionActor.h"

WeakEnemy::WeakEnemy(Sequence* sequence)
	: EnemyActor(sequence)
    , mEnemyState(E_walk)
    , mWeaponComp(nullptr)
{
    mEnemyMove = new EnemyMove(this);
    mEnemyMove->setMoveSpped(32.0f);
	mAnimsc = new AnimSpriteComponent(this);
}

void WeakEnemy::update()
{
    Actor::update();

    fixCollision();

    if (mHpComp->IsKilled()) {
        Actor* a = new ExplosionActor(static_cast<GamePlay*>(mSequence));
        a->setPosition(mPosition);
        a->computeRectangle();
    }
}

void WeakEnemy::changeState(EnemyState nextState)
{
    onExitState(nextState);
    onEnterState(nextState);
}

void WeakEnemy::computeRectangle()
{
	mRectangle.x = mPosition.x - mAnimsc->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
    mRectangle.width = (float)mAnimsc->getTexWidth();
    mRectangle.height = (float)mAnimsc->getTexHeight();
}

void WeakEnemy::onEnterState(EnemyState nextState)
{
    // ステート変更
    mEnemyState = nextState;
    // ステート変更時の処理を呼ぶ
    switch (mEnemyState)
    {
    case E_walk:
        mAnimsc->play(&getAnimation(E_walk));
        break;
    case E_jump:
        mAnimsc->play(&getAnimation(E_jump));
        break;
    case E_attack:
        mAnimsc->play(&getAnimation(E_attack));
        // 攻撃開始
        mWeaponComp->startAttack();
        break;
    }
}

void WeakEnemy::onExitState(EnemyState nextState)
{
    switch (mEnemyState)
    {
    case E_walk:
        break;
    case E_jump:
        break;
    case E_attack:
        mWeaponComp->endAttack();
        break;
    }
}

void WeakEnemy::jump()
{
    if (mEnemyState == E_walk) {
        changeState(E_jump);
        mEnemyMove->setVelocityY(mEnemyMove->getJumpSpeed());
    }
}

void WeakEnemy::fixCollision()
{
    // ステージとの当たり判定
    for (auto& stageRec : static_cast<GamePlay*>(mSequence)->getStage()->getStageRecs()) {
        if (CheckCollisionRecs(mRectangle, stageRec)) {
            Rectangle colRec = GetCollisionRec(mRectangle, stageRec);
            // 横方向の衝突
            if (colRec.width < colRec.height) {
                if (mRectangle.x < colRec.x) mPosition.x -= colRec.width;
                else mPosition.x += colRec.width;

                // 壁にぶつかったときの段差チェック（ジャンプさせる）
                if (mEnemyState != E_jump)
                {
                    const int tileSize = 32;
                    bool isStep = false;
                    if (mPosition.x < stageRec.x && mForward > 0 ||
                        mPosition.x > stageRec.x && mForward < 0) {
                        // 段差が1.5タイル以内ならジャンプ
                        isStep = (stageRec.height <= tileSize * 1.5f);
                    }
                    // 段差の上にスペースがあるか確認
                    Rectangle checkOneAbove = {
                        colRec.x,
                        stageRec.y - tileSize,
                        colRec.width,
                        1.0f
                    };
                    bool isSpaceAboveClear = true;
                    for (const auto& otherStageRec : static_cast<GamePlay*>(mSequence)->getStage()->getStageRecs()) {
                        if (CheckCollisionRecs(checkOneAbove, otherStageRec)) {
                            isSpaceAboveClear = false;
                            break;
                        }
                    }
                    if (isStep && isSpaceAboveClear) {
                        jump();
                        break;
                    }
                }
            }
            // 縦方向の衝突
            else if (colRec.width >= colRec.height) {
                // 上から衝突
                if (mRectangle.y < colRec.y) {
                    mPosition.y -= colRec.height;
                    mEnemyMove->fixFloorCol();
                }
                // 下から衝突
                else {
                    mPosition.y += colRec.height;
                }
            }
            // 当たり判定矩形の更新
            computeRectangle();
        }
    }

    // 武器とプレイヤーの当たり判定
    if (!mWeaponComp->getWeapon()) return;
    
    Actor* player =static_cast<GamePlay*>(mSequence)->getPlayer();
    Rectangle weaponRec = mWeaponComp->getWeapon()->getRectangle();
    if (CheckCollisionRecs(player->getRectangle(), weaponRec)) {
        mWeaponComp->getWeapon()->onHit(player);
    }
}

std::unordered_map<WeakEnemy::EnemyState, Animation> MeleeEnemy::mAnimations = {
    { WeakEnemy::E_walk,   Animation{{}, 24.0f, true} },
    { WeakEnemy::E_jump,   Animation{{}, 24.0f, false} },
    { WeakEnemy::E_attack, Animation{{}, 24.0f, false} },
};

MeleeEnemy::MeleeEnemy(Sequence* sequence)
	: WeakEnemy(sequence)
{
	// アニメーションを設定,諸事情でnewする度に呼んでいる
    // getTextureは必ずロードするわけではないのでとりあえず妥協
	// framesにアニメーションを構成するテクスチャを入れる
    std::vector<Texture2D*> frames;
	// Walk
	frames = { mSequence->getTexture("Assets/testPlayerIdle.png") };
	mAnimations[E_walk].frames = frames;
	mAnimations[E_walk].loop = true;
	// attack
	frames = { mSequence->getTexture("Assets/testPlayerIdle.png") };
	mAnimations[E_attack].frames = frames;
	mAnimations[E_attack].loop = false;
	// jump
	frames = { mSequence->getTexture("Assets/testPlayerIdle.png") };
	mAnimations[E_jump].frames = frames;
	mAnimations[E_jump].loop = false;

    // 攻撃は近接
	mWeaponComp = new SwordComponent(this);
    mEnemyMove->setAttackRange(32.0f);
}

std::unordered_map<WeakEnemy::EnemyState, Animation> RangedEnemy::mAnimations = {
    { WeakEnemy::E_walk,   Animation{{}, 24.0f, true} },
    { WeakEnemy::E_jump,   Animation{{}, 24.0f, false} },
    { WeakEnemy::E_attack, Animation{{}, 24.0f, false} },
};

RangedEnemy::RangedEnemy(Sequence* sequence)
    : WeakEnemy(sequence)
{
    // アニメーションを設定
    std::vector<Texture2D*> frames;

    // Walk
    frames = { mSequence->getTexture("Assets/testPlayerIdle.png") };
    mAnimations[E_walk].frames = frames;
    mAnimations[E_walk].loop = true;
    // attack
    frames = { mSequence->getTexture("Assets/testPlayerIdle.png") };
    mAnimations[E_attack].frames = frames;
    mAnimations[E_attack].loop = false;
    // jump
    frames = { mSequence->getTexture("Assets/testPlayerIdle.png") };
    mAnimations[E_jump].frames = frames;
    mAnimations[E_jump].loop = false;

    // 攻撃は遠距離
    mWeaponComp = new ArrowComponent(this);
    mEnemyMove->setAttackRange(400.0f);
}
