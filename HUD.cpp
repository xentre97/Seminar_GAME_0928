#include "HUD.h"
#include "GamePlay.h"
#include "PlayerActor.h"
#include "HpComponent.h"

HUD::HUD(Sequence* sequence)
	: UIScreen(sequence)
{
	// hpバー
	mHpbar = { 10.0f, 50.0f, 200.0f, 10.0f }; // x,y,width,height
	mCurrentHp = mHpbar;
}

void HUD::update()
{
	// 現在のplayerのhpに応じて,表示する幅を変える
	// TODO: updateで毎回知る必要はない,リファクタリングの余地あり
	mCurrentHp.width = mHpbar.width * 
		static_cast<GamePlay*>(mSequence)->getPlayer()->getHpComp()->GetHpRatio();
}

void HUD::draw()
{
	/* HPbar */
	// "hp" 文字
	DrawText("hp", (int)mHpbar.x, (int)mHpbar.y - 10, 10, BLACK);
	// 背景 (枠)
	DrawRectangleRec(mHpbar, GRAY);
	// HPに応じたバー,20パー以下で色をREDに
	DrawRectangleRec(mCurrentHp, 
		mCurrentHp.width > mHpbar.width / 5 ? GREEN : RED);
}
