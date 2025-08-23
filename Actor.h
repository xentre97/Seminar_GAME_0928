#pragma once
#include <raylib.h>
#include <vector>

/// <summary>
/// ゲームオブジェクト全般の基底クラス
/// </summary>
class Actor
{
public:	
	enum Type
	{
		Eplayer,
		Eenemy,
		Eweapon,
	};
	enum State
	{
		Ealive,
		Edead		// この状態のActorはdeleteされる
	};
	Actor(class Sequence* sequenece, Type type);
	virtual ~Actor();
	
	virtual void input();
	virtual void update();

	void addComponent(class Component* component);
	void removeComponent(class Component* component);

	Type getType() const { return mType; }
	State getState() const { return mState; }
	const Vector2& getPosition() const { return mPosition; }
	virtual const Rectangle& getRectangle() const { return mRectangle; }
	Sequence* getSequence() const { return mSequence; }
	int Forward() { return mForward; }

	void setState(State state) { mState = state; }
	void setPosition(Vector2 pos) { mPosition = pos; }
	void setForward(int forward) { mForward = forward; }

	virtual void computeRectangle() {} ;

protected:
	Type mType;
	State mState;
	Sequence* mSequence;
	Vector2 mPosition;	  // テクスチャの中心座標
	int mForward;		  // +1 : 右向き, -1 : 左向き
	// 当たり判定用の矩形,丸とか色々使うなら派生クラスに持っていく
	Rectangle mRectangle;
	std::vector<class Component*> mComponents;
};

