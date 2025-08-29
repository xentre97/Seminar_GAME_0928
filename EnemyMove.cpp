#include "EnemyMove.h"
#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"

EnemyMove::EnemyMove(EnemyActor* owner)
	: MoveComponent(owner)
	, mEnemy(owner)
	, mAttackRange(32.0f)
{
	mJumpSpeed = -600.0f;
	// ターゲット指定(EnemyActor側から指定してもいい)
	GamePlay* seq = static_cast<GamePlay*>(mOwner->getSequence());
	mTarget = seq->getPlayer();
}

void EnemyMove::update()
{
	// 自由落下への遷移判定を最初に行う
	if (mVelocityY > 0.0f && mEnemy->getEnemyState() != EnemyActor::E_jump) {
		mEnemy->changeState(EnemyActor::E_jump);
	}

	// 重力(垂直移動)
	if (mVelocityY < 600.0f) {
		mVelocityY += mGravity;
	}

	Vector2 ownerPos = mOwner->getPosition();
	Vector2 targetPos = mTarget->getPosition();

	// ステートによって処理が変わる
	switch (mEnemy->getEnemyState())
	{
	case EnemyActor::E_walk:
	{
		// もし射程外なら
		if (abs(ownerPos.x - targetPos.x) > mAttackRange ||
			abs(ownerPos.y - targetPos.y) > mAttackRange) {
			// 自身の位置に対するプレイヤーの位置の方向を取得(自身正なら-1、負なら1に進む)
			int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
			mOwner->setForward(xDirection);
			//水平移動の速度を設定
			mVelocityX = mOwner->getForward() * mMoveSpeed;
		}
		// 射程内なら攻撃状態へ
		else {
			mEnemy->changeState(EnemyActor::E_attack);
			mVelocityX = 0.0f;	// attack時に速度を0にしてみる
		}
		break;
	}
	case EnemyActor::E_jump:
	{
		int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
		mOwner->setForward(xDirection);
		//水平移動の速度を設定
		mVelocityX = mOwner->getForward() * mMoveSpeed;
		break;
	}
	case EnemyActor::E_attack:
	{
		// 射程外になったら(攻撃が終わったらという条件でもよさそう)
		if (abs(ownerPos.x - targetPos.x) > mAttackRange ||
			abs(ownerPos.y - targetPos.y) > mAttackRange) {
			mEnemy->changeState(EnemyActor::E_walk);
			// 自身の位置に対するプレイヤーの位置の方向を取得(自身正なら-1、負なら1に進む)
			int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
			mOwner->setForward(xDirection);
		}
		break;
	}
	}

	//速度を位置に反映
	ownerPos.x += mVelocityX / GetFPS();
	ownerPos.y += mVelocityY / GetFPS();
	mOwner->setPosition(ownerPos);
	// Enemyの移動が完全に終了した後の位置を基に逐次描画するためのRectangle位置明示
	mOwner->computeRectangle();

	// 以下デバッグ用
	switch (mEnemy->getEnemyState())
	{
	case EnemyActor::E_walk: {
		DrawText("Enemy : walk", 700, 150, 30, BLACK); break;
	}
	case EnemyActor::E_jump: {

		DrawText("Enemy : jump", 700, 150, 30, BLACK); break;
	}
	case EnemyActor::E_attack: {
		DrawText("Enemy : attack", 700, 150, 30, BLACK); break;
	}
	}
}

void EnemyMove::fixFloorCol()
{
	// Y方向の速度を初期化
	mVelocityY = 0.0f;
	// 空中からの解消の場合はStateを変更する
	if (mEnemy->getEnemyState() == EnemyActor::E_jump) {
		mEnemy->changeState(EnemyActor::E_walk);
	}
}
