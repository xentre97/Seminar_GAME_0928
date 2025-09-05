#include "HUD.h"
#include "GamePlay.h"
#include "PlayerActor.h"
#include "HpComponent.h"

HUD::HUD(Sequence* sequence)
	: UIScreen(sequence)
{
	// hp�o�[
	mHpbar = { 10.0f, 50.0f, 200.0f, 10.0f }; // x,y,width,height
	mCurrentHp = mHpbar;
}

void HUD::update()
{
	// ���݂�player��hp�ɉ�����,�\�����镝��ς���
	// TODO: update�Ŗ���m��K�v�͂Ȃ�,���t�@�N�^�����O�̗]�n����
	mCurrentHp.width = mHpbar.width * 
		static_cast<GamePlay*>(mSequence)->getPlayer()->getHpComp()->GetHpRatio();
}

void HUD::draw()
{
	/* HPbar */
	// "hp" ����
	DrawText("hp", (int)mHpbar.x, (int)mHpbar.y - 10, 10, BLACK);
	// �w�i (�g)
	DrawRectangleRec(mHpbar, GRAY);
	// HP�ɉ������o�[,20�p�[�ȉ��ŐF��RED��
	DrawRectangleRec(mCurrentHp, 
		mCurrentHp.width > mHpbar.width / 5 ? GREEN : RED);
}
