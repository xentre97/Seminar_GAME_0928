#include "AnimSpriteComponent.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mIsAnimating(false)
	, mCurrentAnim(nullptr)
{
}

void AnimSpriteComponent::update()
{
	SpriteComponent::update();

	if (mCurrentAnim && mIsAnimating)
	{
		// �t���[����i�߂Ă���
		mCurrFrame += mCurrentAnim->fps / GetFPS();
		// �t���[�����𒴂�����
		if (mCurrFrame >= (int)mCurrentAnim->frames.size())
		{
			// ���[�v�A�j���[�V�����̏ꍇ
			if (mCurrentAnim->loop) {
				// �ŏ��̃t���[���ɕ␳�t���Ŗ߂�
				mCurrFrame = fmod(mCurrFrame, (float)mCurrentAnim->frames.size());
			}
			// ���[�v���Ȃ��ꍇ
			else {
				// �ŏI�t���[���ɐݒ�
				mCurrFrame = (float)(mCurrentAnim->frames.size() - 1);
				// �A�j���[�V�����I��
				mIsAnimating = false;
				return;
			}
		}
		// �\���e�N�X�`���X�V
		setTexture(*mCurrentAnim->frames[(int)mCurrFrame]);
	}
}

void AnimSpriteComponent::play(const Animation* anim)
{
	if (!anim) return;
	if (!anim->frames.empty()) {
		mCurrentAnim = anim;
		mCurrFrame = 0.0f;
		mIsAnimating = true;
		setTexture(*anim->frames[0]);
	}
}
