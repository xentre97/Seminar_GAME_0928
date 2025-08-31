#include "AnimSpriteComponent.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(30.0f)
	, mLoopFlag(true)
	, mAnimBegin(0)
	, mAnimEnd(1)
	, mFrameCount(1)
	, mIsAnimating(false)
{
}

void AnimSpriteComponent::update()
{
	SpriteComponent::update();

	// 表示する画像を設定する
	if (mAnimTextures.size() > 0)
	{
		// ループアニメーションの場合
		if (mLoopFlag)
		{
			mCurrFrame += mAnimFPS / (float)GetFPS();
			// 最終フレームまで行ったら,補正して最初から
			if (mCurrFrame >= mFrameCount) {
				mCurrFrame = fmod(mCurrFrame, (float)mFrameCount);
			}
			setTexture(*mAnimTextures[static_cast<int>(mCurrFrame + mAnimBegin)]);
		}
		// ループ以外の場合
		else if (!mLoopFlag && mIsAnimating)
		{
			mCurrFrame += mAnimFPS / (float)GetFPS();
			// アニメーション終了時の処理
			if (mCurrFrame >= mFrameCount)
			{
				mCurrFrame = 0.0f;
				mIsAnimating = false;
			}
			setTexture(*mAnimTextures[static_cast<int>(mCurrFrame + mAnimBegin)]);
		}
		// ループでないアニメーションの終了時、先頭のテクスチャに設定する
		else if (!mLoopFlag && !mIsAnimating)
		{
			setTexture(*mAnimTextures[0]);
		}
	}
}

void AnimSpriteComponent::setAnimTextures(std::vector<Texture2D*> textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// 先頭のテクスチャを設定
		mCurrFrame = 0.0f;
		setTexture(*mAnimTextures[0]);
	}
}

void AnimSpriteComponent::play(int begin, int end, bool loop, float fps)
{
	mAnimBegin = begin;
	mAnimEnd = end;
	mFrameCount = end - begin + 1;
	mAnimFPS = fps;
	mLoopFlag = loop;
	mCurrFrame = 0.0f;
	mIsAnimating = true;

	// 開始時のフレームを設定
	setTexture(*mAnimTextures[mAnimBegin]);
}