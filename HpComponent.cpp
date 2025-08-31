#include "HpComponent.h"
#include <iostream>

HpComponent::HpComponent(Actor* owner, float maxHp)
	: Component(owner)
	, mMaxHp(maxHp)
	, mCurHp(maxHp)
{
}

bool HpComponent::TakeDamage(float damage)
{
	// hpは0以下にならない
	mCurHp -= damage;
	if (mCurHp <= 0.0f) {
		mCurHp = 0.0f;
		return true;
	}
	//デバッグ用
	std::cout << "mCurHp=" << mCurHp << std::endl;
	return false;
}

void HpComponent::Recover(float recover)
{
	// hpはmMaxHpを超えない
	float diff = mMaxHp - mCurHp;
	if (diff <= recover) {
		recover = diff;
	}
	mCurHp += recover;
}

void HpComponent::AddMaxHp(float add)
{
	mMaxHp += add;
	mCurHp += add;
}
