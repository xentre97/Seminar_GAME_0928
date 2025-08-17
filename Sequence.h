#pragma once
#include <vector>
#include <raylib.h>
#include "Actor.h"

/// <summary>
/// シーケンスの親クラス
/// タイトル,ゲーム画面,ゲームオーバーなどを子に持つ
/// 
/// </summary>
class Sequence
{
public:
    virtual ~Sequence();

    virtual void input();
    virtual void update();
    virtual void draw();

    // シーン遷移が必要になったら次のシーケンスを返す
    virtual Sequence* nextSequence() { return nullptr; }

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

protected:
    // そのシーケンスに存在するアクターの配列
    std::vector<class Actor*> mActors;
    // コンストラクタをprotectedにして直接 new Sequenceできないように
    Sequence() {};
};
