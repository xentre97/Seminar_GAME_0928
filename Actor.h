#pragma once
#include <raylib.h>
#include <vector>

/// <summary>
/// ゲームオブジェクト全般の基底クラス
/// </summary>
class Actor
{
public:	
	Actor(class Sequence* sequenece);
	virtual ~Actor();
	
	virtual void input();
	virtual void update();
	virtual void draw();

	void addComponent(class Component* component);

	virtual const Rectangle& getRectangle() const { return mRectangle; }
	const Vector2& getPosition() const { return mPosition; }
	Sequence* getSequence() const { return mSequence; }

	void setPosition(Vector2 pos) { mPosition = pos; }

protected:
	Sequence* mSequence;
	Texture2D mTexture;	  //
	Vector2 mPosition;	  // テクスチャの中心座標
	// 当たり判定用の矩形,丸とか色々使うなら派生クラスに持っていく
	Rectangle mRectangle;
	std::vector<class Component*> mComponents;
};

