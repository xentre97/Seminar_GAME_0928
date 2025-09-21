#include "WeakEnemy.h"
#include "GamePlay.h"
#include "Stage.h"

#include "EnemyMove.h"
#include "AnimSpriteComponent.h"
#include "HpComponent.h"
#include "AttackComponent.h"
// weapon
#include "WeaponActor.h"
#include "PlayerActor.h"
#include "ArrowActor.h"
#include "ItemActor.h"

WeakEnemy::WeakEnemy(Sequence* sequence)
	: EnemyActor(sequence)
    , mEnemyState(E_walk)
{
    mEnemyMove = new EnemyMove(this);
    mEnemyMove->setMoveSpped(32.0f);
	mAnimsc = new AnimSpriteComponent(this);
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
    // �X�e�[�g�ύX
    mEnemyState = nextState;
    // �X�e�[�g�ύX���̏������Ă�
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
        attack();
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
        //mWeaponComp->endAttack();
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

void WeakEnemy::computeAttackRectPos(Rectangle& rec)
{
    if (mForward > 0) rec.x = mRectangle.x + mRectangle.width;
    else rec.x = mRectangle.x - rec.width;
    rec.y = mRectangle.y;
}

void WeakEnemy::fixCollision()
{
    // �X�e�[�W�Ƃ̓����蔻��
    for (auto& stageRec : static_cast<GamePlay*>(mSequence)->getStage()->getStageRecs()) {
        if (CheckCollisionRecs(mRectangle, stageRec)) {
            Rectangle colRec = GetCollisionRec(mRectangle, stageRec);
            // �������̏Փ�
            if (colRec.width < colRec.height) {
                if (mRectangle.x < colRec.x) mPosition.x -= colRec.width;
                else mPosition.x += colRec.width;

                // �ǂɂԂ������Ƃ��̒i���`�F�b�N�i�W�����v������j
                if (mEnemyState != E_jump)
                {
                    const int tileSize = 32;
                    bool isStep = false;
                    if (mPosition.x < stageRec.x && mForward > 0 ||
                        mPosition.x > stageRec.x && mForward < 0) {
                        // �i����1.5�^�C���ȓ��Ȃ�W�����v
                        isStep = (stageRec.height <= tileSize * 1.5f);
                    }
                    // �i���̏�ɃX�y�[�X�����邩�m�F
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
            // �c�����̏Փ�
            else if (colRec.width >= colRec.height) {
                // �ォ��Փ�
                if (mRectangle.y < colRec.y) {
                    mPosition.y -= colRec.height;
                    mEnemyMove->fixFloorCol();
                }
                // ������Փ�
                else {
                    mPosition.y += colRec.height;
                }
            }
            // �����蔻���`�̍X�V
            computeRectangle();
        }
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
	// �A�j���[�V������ݒ�,�������new����x�ɌĂ�ł���
    // getTexture�͕K�����[�h����킯�ł͂Ȃ��̂łƂ肠�����Ë�
	// frames�ɃA�j���[�V�������\������e�N�X�`��������
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

    // �U���͋ߐ�
    mAttackComp = new AttackComponent(this);
    // �U�����̐ݒ�
    mAttackInfo.damage = 10.0f;
    mAttackInfo.duration = 1.0f;
    mAttackInfo.colRect.width = 16.0f;
    mAttackInfo.colRect.height = 16.0f;
    computeAttackRectPos(mAttackInfo.colRect);
    mAttackInfo.knockBack = 0.0f;
    mAttackInfo.targetType = Actor::Type::Eplayer;

    mEnemyMove->setAttackRange(32.0f);
}

void MeleeEnemy::update()
{
    Actor::update();
    fixCollision();
    computeAttackRectPos(mAttackInfo.colRect);

    if (mHpComp->IsKilled()) {
        if(GetRandomValue(1, 100) > 50) {
            Actor* item = new HealingItem(mSequence);
            item->setPosition(mPosition);
            item->computeRectangle();
        }
    }
}

void MeleeEnemy::attack()
{
    mAttackComp->startAttack(&mAttackInfo);
}

std::unordered_map<WeakEnemy::EnemyState, Animation> RangedEnemy::mAnimations = {
    { WeakEnemy::E_walk,   Animation{{}, 24.0f, true} },
    { WeakEnemy::E_jump,   Animation{{}, 24.0f, false} },
    { WeakEnemy::E_attack, Animation{{}, 24.0f, false} },
};

RangedEnemy::RangedEnemy(Sequence* sequence)
    : WeakEnemy(sequence)
{
    // �A�j���[�V������ݒ�
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

    mEnemyMove->setAttackRange(400.0f);
}

void RangedEnemy::update()
{
    Actor::update();
    fixCollision();
    if (mHpComp->IsKilled()) {
        if (GetRandomValue(1, 100) > 50) {
            Actor* item = new HealingItem(mSequence);
            item->setPosition(mPosition);
            item->computeRectangle();
        }
    }
}

void RangedEnemy::onEnterState(EnemyState nextState)
{
    WeakEnemy::onEnterState(nextState);

    if (nextState == E_attack) {
         ArrowActor* a = new ArrowActor(mSequence, Actor::Eenemy);
         a->setPosition(mPosition);
         a->computeRectangle();
         a->setForward(mForward);
         a->setVelocity(Vector2{ 700.0f * mForward, 0.0f });
    }
}
