#pragma once
#include <raylib.h>
#include <vector>

/// <summary>
/// �Q�[���I�u�W�F�N�g�S�ʂ̊��N���X
/// </summary>
class Actor
{
public:	
	enum Type
	{
		Eplayer,
		Eenemy,
		Eweapon,
		EstageObject,
		Eexplosion,
	};
	enum State
	{
		Ealive,
		Edead		// ���̏�Ԃ�Actor��delete�����
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
	int getForward() const { return mForward; }

	void setState(State state) { mState = state; }
	// ���̊֐���ǂ񂾌��computeRectangle()���ĂԂ悤�ɂ���
	void setPosition(Vector2 pos) { mPosition = pos; }
	void setForward(int forward) { mForward = forward; }
	// setPosition�Ɠ����ɌĂяo��
	virtual void computeRectangle() {} ;

protected:
	Type mType;
	State mState;
	Sequence* mSequence;
	Vector2 mPosition;	  // �e�N�X�`���̒��S���W
	int mForward;		  // +1 : �E����, -1 : ������
	// �����蔻��p�̋�`,�ۂƂ��F�X�g���Ȃ�h���N���X�Ɏ����Ă���
	Rectangle mRectangle;
	std::vector<class Component*> mComponents;
};

