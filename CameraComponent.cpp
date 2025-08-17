#include "CameraComponent.h"
#include "Actor.h"
#include "GamePlay.h"

CameraComponent::CameraComponent(Actor* owner)
	: Component(owner)
	, mCamera({ 0 })
	, mStageWidth(0.0f)
	, mStageHeight(0.0f)
{
	mCamera.zoom = 1.0f;
	mCamera.rotation = 0.0f;
	mCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	// getSequenceは設計として微妙かもしれない
	// stageクラスをシングルトンで作るとかの方がいいかも？
	mStageWidth = static_cast<GamePlay*>(mOwner->getSequence())->getStageWidth();
}

void CameraComponent::update()
{
	// ownerのx座標を追う
	float targetX = mOwner->getPosition().x;
	
	float centerX = GetScreenWidth() / 2.0f;

	// プレイヤーが画面の左端から中央に来るまではカメラを固定
	if (mOwner->getPosition().x < centerX) {
		targetX = centerX;
	}
	// ステージ右端に到達したらカメラ固定
	if (mOwner->getPosition().x > mStageWidth - centerX) {
		targetX = mStageWidth - centerX;
	}
	mCamera.target = { targetX, GetScreenHeight() / 2.0f };
}
