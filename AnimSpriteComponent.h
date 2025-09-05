#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <raylib.h>
#include "Animation.h"

/// <summary>
/// �A�j���[�V�����t���X�v���C�g�̕`��
/// </summary>
class AnimSpriteComponent :
    public SpriteComponent
{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
    // ���̃t���[�����ƂɃA�j���[�V�������X�V component����override
    void update() override;

    // �A�j���[�V�������Đ�����
    void play(const Animation* anim);

    bool isAnimating() const { return mIsAnimating; }

private:
    // �A�j���[�V�����̃}�b�v
    std::unordered_map<std::string, Animation> mAnimations;
    const Animation* mCurrentAnim;  // ���݂̃A�j���[�V����
    // ���ݕ\�����Ă���t���[��
    float mCurrFrame;
    bool mIsAnimating;
};

// TODO: ���t�@�N�^�����O
// ���͕����̃A�j���[�V���������̃R���|�[�l���g�Ɏ������Ă��邪�A�O���ɃA�j���[�V������������,
// ���̃R���|�[�l���g�ɓn���čĐ�����d�g�݂ł��悳����