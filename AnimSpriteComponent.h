#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <raylib.h>

/// <summary>
/// アニメーション情報
/// </summary>
struct Animation
{
    std::string name;                  // アニメーション名
    std::vector<Texture2D*> frames;    // 各フレームのテクスチャ
    float fps;                         // 再生FPS
    bool loop;                         // ループアニメーションかどうか
};

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
    void setAnimTextures(std::vector<Texture2D*> textures);
    void setTexturesMap(std::string name, std::vector<Texture2D*> textures);
    // アニメーションを再生する
    void play(int begin, int end, bool loop, float fps = 60.0f);

    // アニメーションを追加
    void addAnimation(const std::string& name,
        const std::vector<Texture2D*>& frames,
        float fps = 60.0f,
        bool loop = true);
    // アニメーションを再生する
    void play(std::string animName);

    bool isAnimating() const { return mIsAnimating; }

private:
    std::unordered_map<std::string, Animation> mAnimations;
    Animation* mCurrentAnim;  // 現在のアニメーション
    // アニメーションでのすべてのテクスチャ
    std::vector<Texture2D*> mAnimTextures;
    std::unordered_map<std::string, std::vector<Texture2D*>> mTexturesMap;
    // 現在表示しているフレーム
    float mCurrFrame;
    // アニメーションのFPS 動的に変更可能
    float mAnimFPS;
    bool mLoopFlag;
    int mAnimBegin;
    int mAnimEnd;
    int mFrameCount;
    bool mIsAnimating;
};