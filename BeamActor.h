#pragma once
//===============================================
// BeamActor.h
//  短時間だけ出る直線ビーム。当たりは大きめの長方形。
//===============================================
#include "Actor.h"
#include "DamageTags.h"

class GamePlay;

class BeamActor : public Actor {
public:
    // start  : ボスの口元など、ビームの起点
    // forward: 向き（+1 右 / -1 左）
    BeamActor(GamePlay* seq, Vector2 start, int forward);

    void update() override;          // ライフタイムと当たりのみ
    void computeRectangle() override;// 向きに応じて前方に長い矩形を張る

    DamageTag tag = DamageTag::Beam;

private:
    GamePlay* mGP   = nullptr;
    float     mLife = 0.25f;         // 持続 0.25 秒（瞬間的）
};
