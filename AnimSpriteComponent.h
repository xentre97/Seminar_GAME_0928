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

    // アニメーションを追加
    void addAnimation(const std::string& name,
        const std::vector<Texture2D*>& frames,
        float fps = 60.0f,
        bool loop = true);
    // アニメーションを再生する
    void play(std::string animName);

    bool isAnimating() const { return mIsAnimating; }

private:
    // アニメーションのマップ
    std::unordered_map<std::string, Animation> mAnimations;
    Animation* mCurrentAnim;  // 現在のアニメーション
    // 現在表示しているフレーム
    float mCurrFrame;
    bool mIsAnimating;
};

// TODO: リファクタリング
// 今は複数のアニメーションをこのコンポーネントに持たせているが、外部にアニメーションを持たせ,
// このコンポーネントに渡して再生する仕組みでもよさそう