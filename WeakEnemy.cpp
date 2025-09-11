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
        // �U���J�n
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

    // ����ƃv���C���[�̓����蔻��
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

    // �U���͉�����
    mWeaponComp = new ArrowComponent(this);
    mEnemyMove->setAttackRange(400.0f);
}
