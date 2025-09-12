#pragma once
#include <raylib.h>
#include <vector>
#include "Actor.h"

struct AttackInfo
{
	float damage;		// ダメージ量
	float duration;		// 攻撃持続時間 アニメーションとは独立して管理する
	Rectangle colRect;	// 攻撃範囲 当たり判定用の矩形
	float knockBack;	// ノックバック力
	int forward;		// 攻撃の向き

	Actor::Type targetType;   // 攻撃対象の種類
};