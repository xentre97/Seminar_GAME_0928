#include "AnimSpriteComponent.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(30.0f)
	, mLoopFlag(true)
	, mAnimBegin(0)
	, mAnimEnd(1)
	, mIsAnimating(false)
{
}

void AnimSpriteComponent::update()
{
	SpriteComponent::update();

	if (mAnimTextures.size() > 0)
	{
		// ループアニメーション
		if (mLoopFlag)
		{
			mCurrFrame += mAnimFPS / (float)GetFPS();
			int frameCount = mAnimEnd - mAnimBegin + 1;
		/*if (mCurrFrame >= mAnimEnd + 1)
		{
			mCurrFrame = mCurrFrame - (mAnimEnd + 1);
		}*/

			// ループ補正
			if (mCurrFrame >= frameCount)
			{
				mCurrFrame = fmod(mCurrFrame, (float)frameCount);
			}
			setTexture(mAnimTextures[static_cast<int>(mCurrFrame + mAnimBegin)]);
		}
		// ループじゃない場合
		else if (!mLoopFlag && mIsAnimating)
		{
			mCurrFrame += mAnimFPS / (float)GetFPS();
			if (mCurrFrame > mAnimEnd - mAnimBegin)
			{
				mCurrFrame = 0.0f;
				mIsAnimating = false;
			}
			setTexture(mAnimTextures[static_cast<int>(mCurrFrame + mAnimBegin)]);
		}
		// ループでないアニメーションの終了時、先頭のテクスチャに設定する
		else if (!mLoopFlag && !mIsAnimating)
		{
			setTexture(mAnimTextures[0]);
		}
	}
}

void AnimSpriteComponent::setAnimTextures(const std::vector<Texture2D>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// 先頭のテクスチャを設定
		mCurrFrame = 0.0f;
		setTexture(mAnimTextures[0]);
	}
}

void AnimSpriteComponent::play(int begin, int end, bool loop, float fps)
{
	mAnimBegin = begin;
	mAnimEnd = end;
	mAnimFPS = fps;
	mLoopFlag = loop;
	mCurrFrame = 0.0f;
	mIsAnimating = true;

	// 開始時のフレームを設定
	setTexture(mAnimTextures[mAnimBegin]);
}