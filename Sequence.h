#pragma once
#include <vector>
#include <string>
#include <unordered_map>
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

    virtual void unloadData();

    // シーン遷移が必要になったら次のシーケンスを返す
    virtual Sequence* nextSequence() { return nullptr; }

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    Texture2D* getTexture(const std::string& fileName);

protected:
    // そのシーケンスに存在するアクターの配列
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors; // mActorsへの追加待ちのActor
    // そのシーケンスでロードしたテクスチャ達
    std::unordered_map<std::string, Texture2D> mTextures;
    bool mUpdatingActors = false;
    // コンストラクタをprotectedにして直接 new Sequenceできないように
    Sequence() {};
};
