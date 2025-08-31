#include "ArrowActor.h"
#include "Sequence.h"

#include "AnimSpriteComponent.h"

ArrowActor::ArrowActor(Sequence* sequence, Type type, Type owner)
    : WeaponActor(sequence, type, owner)
{
    // 初期テクスチャと当たり
    //Texture2D tex0 = mSequence->getTexture("arrow01.png");
    Texture2D* tex0 = mSequence->getTexture("testSword01.png");
    mPosition = Vector2{ 500.0f, 300.0f };
    mRectangle = {
        mPosition.x - tex0->width / 2.0f,
        mPosition.y - tex0->height / 2.0f,
        (float)tex0->width,
        (float)tex0->height
    };

    // 簡易アニメ（お好みでフレーム追加）
    mAnimsc = new AnimSpriteComponent(this);
    std::vector<Texture2D*> frames = {
        tex0,
        /*mSequence->getTexture("arrow02.png"),
        mSequence->getTexture("arrow03.png"),
        */
    };
    mAnimsc->addAnimation("Normal", frames, 24.0f, false);
}

void ArrowActor::input()
{
    Actor::input();
}

void ArrowActor::update()
{
    Actor::update();

    if (mDead) return;

    const float dt = GetFrameTime();
    mAge += dt;

    // 移動
    mPosition.x += mVelocity.x * dt;
    mPosition.y += mVelocity.y * dt;

    computeRectangle();

    // 画面外 or 寿命で終了
    const float halfW = mAnimsc->getTexWidth()  * 0.5f;
    const float halfH = mAnimsc->getTexHeight() * 0.5f;
    const int   sw = GetScreenWidth();
    const int   sh = GetScreenHeight();

    const bool out =
        (mPosition.x + halfW < 0) || (mPosition.x - halfW > sw) ||
        (mPosition.y + halfH < 0) || (mPosition.y - halfH > sh);

    if (out || mAge >= mLifeTime) {
        mDead = true;
        // 必要ならここで「消滅演出」や「当たり無効化」を入れる
        // （メモリ破棄はプロジェクトのActor管理方針に合わせて）
    }
}

void ArrowActor::onStartAttack()
{
    mAnimsc->play("Normal"); // 矢は回転/ヒラヒラ想定でループ
}

void ArrowActor::computeRectangle()
{
    mRectangle.x = mPosition.x - mAnimsc->getTexWidth()  / 2.0f;
    mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
}
