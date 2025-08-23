#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <raylib.h>

/// <summary>
/// アニメーション付きスプライトの描画
/// </summary>
class AnimSpriteComponent :
    public SpriteComponent
{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
    // 一定のフレームごとにアニメーションを更新 componentからoverride
    void update() override;
    // アニメーションに使うテクスチャの設定
    void setAnimTextures(const std::vector<Texture2D>& textures);
    // アニメーションを再生する
    void play(int begin, int end, bool loop, float fps = 60.0f);

    bool isAnimating() const { return mIsAnimating; }

private:
    // アニメーションでのすべてのテクスチャ
    std::vector<Texture2D> mAnimTextures;
    // 現在表示しているフレーム
    float mCurrFrame;
    // アニメーションのFPS 動的に変更可能
    float mAnimFPS;
    bool mLoopFlag;
    int mAnimBegin;
    int mAnimEnd;
    bool mIsAnimating;
};