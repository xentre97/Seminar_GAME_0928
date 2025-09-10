#pragma once
#include "Actor.h"

/// <summary>
/// �����A�N�^�[,�G���|���ꂽ���Ȃǂɐ������� �G�t�F�N�g�݂����ȃC���[�W
/// �ȉ�Actor�ɂ������R
/// �����蔻��p��`��e�N�X�`����Ɨ����Ĉ�������
/// �܂��G�ȊO�ɂ������邱�Ƃ���,�Ɨ����Ĉ�����������
/// ���Ƃ��₷�����Ǝv��������
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

