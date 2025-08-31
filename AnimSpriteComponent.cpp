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

	if (mCurrentAnim && mIsAnimating)
	{
		// フレームを進めていく
		mCurrFrame += mCurrentAnim->fps / GetFPS();
		// フレーム数を超えたら
		if (mCurrFrame >= (int)mCurrentAnim->frames.size())
		{
			// ループアニメーションの場合
			if (mCurrentAnim->loop) {
				// 最初のフレームに補正付きで戻る
				mCurrFrame = fmod(mCurrFrame, (float)mCurrentAnim->frames.size());
			}
			// ループしない場合
			else {
				// 最終フレームに設定
				mCurrFrame = (float)(mCurrentAnim->frames.size() - 1);
				// アニメーション終了
				mIsAnimating = false;
			}
		}
		// 表示テクスチャ更新
		setTexture(*mCurrentAnim->frames[(int)mCurrFrame]);
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

void AnimSpriteComponent::setTexturesMap(std::string name, std::vector<Texture2D*> textures)
{
	mTexturesMap[name] = textures;
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

void AnimSpriteComponent::addAnimation(const std::string& name, const std::vector<Texture2D*>& frames, float fps, bool loop)
{
	// アニメーションを作成し、内容を設定
	Animation anim;
	anim.name = name;
	anim.frames = frames;
	anim.fps = fps;
	anim.loop = loop;
	// マップに追加
	mAnimations[name] = anim;
}

void AnimSpriteComponent::play(std::string animName)
{
	auto it = mAnimations.find(animName);
	if (it != mAnimations.end()) {
		mCurrentAnim = &it->second;
		mCurrFrame = 0.0f;
		mIsAnimating = true;
		setTexture(*mCurrentAnim->frames[0]);
	}
}
