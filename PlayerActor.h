#pragma once
#include "Actor.h"
#include <map>
#include "PlayerControl.h"

/// <summary>
/// PlayerActorクラス
/// </summary>
class PlayerActor :
    public Actor
{
public:

    PlayerActor(class Sequence* sequence);

    void input() override;
    void update() override;
    void draw() override;

    const Camera2D& getCamera() const ;
    class PlayerControl& getPlayerControl();
    void computeRectangle();
    void setMoveTexture(PlayerControl::MoveState s);

private:
    class CameraComponent* mCameraComp;
    class PlayerControl* mPlayerControl;
    std::map<PlayerControl::MoveState, Texture2D> mMoveTextures;
};

// 挙動
// AD : 移動
// Space : ジャンプ
// Shift + 移動 : ダッシュ
// S : ガード(移動,ジャンプ不可)
// W : 攻撃

// 状態 状態によってアニメーションを変える
// 攻撃...1枚の絵
// 移動...ループアニメーション
// ジャンプ...ジャンプの絵
// ガード...一枚の絵
// 見てる向き(一旦保留)