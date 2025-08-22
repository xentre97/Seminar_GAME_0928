#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner)
	: Component(owner)
	, mGravity(1.0f)
	, mMoveSpeed(180.0f)
	, mJumpSpeed(-20.0f)
	, mVelocityX(0.0f)
	, mVelocityY(0.0f)
	, mIsJumping(false)
{
}

