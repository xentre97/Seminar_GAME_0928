//===============================================
// BeamActor.cpp
//===============================================
#include "BeamActor.h"
#include "GamePlay.h"
#include "PlayerActor.h"
#include "HpComponent.h"   // TakeDamage() のため

BeamActor::BeamActor(GamePlay* seq, Vector2 start, int forward)
    : Actor(seq, Actor::Eweapon)
    , mGP(seq)
    , mLife(0.25f)
{
    setPosition(start);
    computeRectangle();
    mForward = forward;
}

void BeamActor::update() {
    const float dt = GetFrameTime();

    // 寿命管理
    mLife -= dt;
    if (mLife <= 0.0f) {
        setState(Actor::Edead);
        return;
    }

    // 当たりは毎フレーム更新（カメラ・親の動きに追従したい場合）
    computeRectangle();

    // プレイヤーにヒット中は連続ダメージ（短時間）
    auto* player = mGP->getPlayer();
    if (CheckCollisionRecs(player->getRectangle(), mRectangle)) {
        player->getHpComp()->TakeDamage(18.0f);
    }
}

void BeamActor::computeRectangle() {
    // 直線ビーム：向きに応じて横に長い当たり（高さ 24）
    if (mForward >= 0) {
        mRectangle = { mPosition.x,           mPosition.y - 12.0f, 900.0f, 24.0f };
    } else {
        mRectangle = { mPosition.x - 900.0f,  mPosition.y - 12.0f, 900.0f, 24.0f };
    }
}
