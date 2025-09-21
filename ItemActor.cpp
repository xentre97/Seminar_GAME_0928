#include "ItemActor.h"
#include "GamePlay.h"

#include "SpriteComponent.h"
// ÉvÉåÉCÉÑÅ[
#include "PlayerActor.h"
#include "HpComponent.h"

ItemActor::ItemActor(Sequence* sequence)
	: Actor(sequence, Type::Eitem)
{
	mSpriteComp = new SpriteComponent(this);
}

void ItemActor::update()
{
	Actor::update();
	
	Rectangle playerRec = static_cast<GamePlay*>(mSequence)->getPlayer()->getRectangle();
	if (CheckCollisionRecs(playerRec, mRectangle)) {
		onAcquired();
		setState(Edead);
	}
}

void ItemActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mRectangle.width / 2.0f;
	mRectangle.y = mPosition.y - mRectangle.height / 2.0f;
}

HealingItem::HealingItem(Sequence* sequence)
	: ItemActor(sequence)
	, mHealAmount(20.0f)
{
	Texture2D* tex = mSequence->getTexture("Assets/testHealingItem.png");
	mSpriteComp->setTexture(*tex);
	mRectangle.width = tex->width;
	mRectangle.height = tex->height;
}

void HealingItem::onAcquired()
{
	static_cast<GamePlay*>(mSequence)->getPlayer()->
		getHpComp()->Recover(mHealAmount);
}
