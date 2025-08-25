#include "EnemyMove.h"
#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"

EnemyMove::EnemyMove(EnemyActor* owner)
	: MoveComponent(owner)
	, mEnemy(owner)
	, mAttackRange(5)
{
	mJumpSpeed = -300.0f;
	// ターゲット指定(EnemyActor側から指定してもいい)
	GamePlay* seq = static_cast<GamePlay*>(mOwner->getSequence());
	mTarget = seq->getPlayer();
}

void EnemyMove::update()
{
	float deltaTime = GetFrameTime();
	
	// 重力(垂直移動)
	if (mVelocityY < 600.0f) {
		mVelocityY += mGravity;
	}

	Vector2 ownerPos = mOwner->getPosition();
	Vector2 targetPos = mTarget->getPosition();

	// もし射程外なら
	if (abs(ownerPos.x - targetPos.x) > mAttackRange || 
		abs(ownerPos.y - targetPos.y) > mAttackRange) {
		// 自身の位置に対するプレイヤーの位置の方向を取得(自身正なら-1、負なら1に進む)
		int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
		mOwner->setForward(xDirection);

		//鉛直移動
		//if()
	}
	else {
		// 静止しているので、攻撃の実装
		mOwner->setForward(0);
	}

	//水平移動の速度を設定
	int forward = mOwner->getForward();
	mVelocityX = forward * mMoveSpeed;

	//速度を位置に反映
	ownerPos.x += mVelocityX / GetFPS();
	ownerPos.y += mVelocityY / GetFPS();

	mOwner->setPosition(ownerPos);
	// Enemyの移動が完全に終了した後の位置を基に逐次描画するためのRectangle位置明示
	mOwner->computeRectangle();
}

void EnemyMove::fixFloorCol()
{
	// めっちゃとりあえず
	if(mVelocityY != mJumpSpeed) mVelocityY = 0.0f;
}
