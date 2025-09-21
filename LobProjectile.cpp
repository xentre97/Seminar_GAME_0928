//===============================================
// LobProjectile.cpp
//===============================================
#include "LobProjectile.h"
#include "GamePlay.h"
#include "PlayerActor.h"
#include "HpComponent.h"     // TakeDamage() 呼び出しのために必要

LobProjectileActor::LobProjectileActor(GamePlay* seq, Vector2 pos, int forward)
    : Actor(seq, Actor::Eweapon)
    , mGP(seq)
    , mLife(4.0f)
{
    // 位置と初速
    setPosition(pos);
    mVel      = { 250.0f * (float)forward, -380.0f };  // 横に 250、上向きに 380
    mForward  = forward;
}

void LobProjectileActor::update() {
    const float dt = GetFrameTime();

    // 簡易重力
    mVel.y     += 980.0f * dt;
    mPosition.x = mPosition.x + mVel.x * dt;
    mPosition.y = mPosition.y + mVel.y * dt;

    // 当たり更新＆寿命
    computeRectangle();
    mLife -= dt;
    if (mLife <= 0.0f) {
        setState(Actor::Edead);
        return;
    }

    // プレイヤーにヒットしたらダメージを与えて消滅
    auto* player = mGP->getPlayer();
    if (CheckCollisionRecs(player->getRectangle(), mRectangle)) {
        player->getHpComp()->TakeDamage(20.0f);
        setState(Actor::Edead);
    }
}

void LobProjectileActor::computeRectangle() {
    // 16x16 の簡易当たり
    mRectangle = { mPosition.x - 8.0f, mPosition.y - 8.0f, 16.0f, 16.0f };
}
