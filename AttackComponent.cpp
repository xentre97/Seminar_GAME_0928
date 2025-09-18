#include "AttackComponent.h"
#include <raymath.h>

#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"

#include "HpComponent.h"

AttackComponent::AttackComponent(Actor* owner)
	: Component(owner)
	, mTimer(0.0f)
	, mActive(false)
	, mCurInfo(nullptr)
{
}

void AttackComponent::update()
{
	if (mActive) {
		/* �U���I������ */
		mTimer += GetFrameTime();
		// �U�����Ԃ��߂������active��
		if (mTimer > mCurInfo->duration) {
			mActive = false;
		}
	
		switch (mCurInfo->targetType)
		{
		// �G�ւ̍U���̏ꍇ
		case Actor::Type::Eenemy:
			processAttackEnemy();
			break;
		case Actor::Type::Eplayer:
			processAttackPlayer();
			break;
		}
		//kb move
		for (int i = mKnockbackTargets.size() - 1; i >= 0; --i) {
			auto& info = mKnockbackTargets[i];

			//�^�[�Q�b�g���L�����`�F�b�N
			if (info.target->getState() == Actor::Edead) {
				mKnockbackTargets.erase(mKnockbackTargets.begin() + i);
				continue;
			}
			info.timer -= GetFrameTime();

			if (info.timer <= 0.0f) {
				mKnockbackTargets.erase(mKnockbackTargets.begin() + i);
			}
			else {
				Vector2 currentPos = info.target->getPosition();
				Vector2 moveAmount = Vector2Scale(info.velocity, GetFrameTime());
				Vector2 newPos = Vector2Add(currentPos, moveAmount);

				info.target->setPosition(newPos);
				info.target->computeRectangle();
			}
		}
	}
}

void AttackComponent::startAttack(AttackInfo* info)
{
	mActive = true;
	mTimer = 0.0f;
	mCurInfo = info; // ���݂̍U����ݒ�
	mHitActors.clear();
}

void AttackComponent::processAttackEnemy()
{
	// �撣���ēG�������Ă���B�{����ActorManager���玝���Ă���݂����Ȏd�g�݂ɂ������@
	std::vector<EnemyActor*> enemies = 
		static_cast<GamePlay*>(mOwner->getSequence())->getEnemies();
	for (auto enemy : enemies) {
		// ���� �F�X��₪����
		//
		// 1.�m�b�N�o�b�N��Ԃ���Ȃ��G�ɂ͓��ĂȂ�
		//  �ȒP����,�m�b�N�o�b�N���Ԃ��Z�����Ƃ��O��B��肠������炵���Ȃ�C������
		// 2.�q�b�g���X�g
		//  ��ԗ��z�ł͂���B�G�������o��������Ȃ�O(n^2)���C�ɂȂ邩��(�����債�ĕς��Ȃ����͂���)
		//  �ǋL�F�q�b�g���X�g���̓G��delete���ꂽ���̏������K�v
		// 3.�U���t���[�����ɒN���ɓ���������,mActive = false�ɂ���
		//  �ꉞ�����ɕ����ɓ��Ă���̂�����,�m�b�N�o�b�N���̓G�ɂ����Ă���
		//  �c�荁�I�ȍU�����肪�Ȃ��̂ŁA�_�b�V���U���̋����͐S�z

		// �m�b�N�o�b�N��Ԃ��܂��Ȃ��̂�,�Ƃ肠����3���̗p���܂���
		if (enemy->getHpComp()->isInvincible()) {
			continue;
		}
		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			mHitActors.push_back(enemy);
			//Kb
			Vector2 attackerPos = mOwner->getPosition();
			Vector2 targetPos = enemy->getPosition();
			Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));

			float upspeed = 2.0f;
			direction.y -= upspeed;
			direction = Vector2Normalize(direction);

			KnockbackInfo info;
			info.target = enemy; //Knockback�\���̂�target��enemy��ݒ�
			info.timer = 0.2f;
			float speed = 300.0f;
			info.velocity = Vector2Scale(direction, speed);

			mKnockbackTargets.push_back(info);

			if (enemy->getHpComp()->TakeDamage(mCurInfo->damage)) {
				enemy->setState(Actor::Edead);
				mActive = false;
			}
		}
		
		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			if (enemy->getHpComp()->TakeDamage(mCurInfo->damage)) {
				enemy->setState(Actor::Edead);
				mActive = false;
			}
		}

		// �Ȃ��A�G���G�ɍU�����鎖���\�Ȃ͂�
		// �������g���U���������Ȃ��Ȃ�,component��owner�Ƃ͓�����Ȃ��悤�ɂ����ɏ����΂���
		// �m�b�N�o�b�Nenter���ɓG��startAttack()���Ăׂ�,�G�ƓG�̃m�b�N�o�b�N�������\���낤
	}
}

void AttackComponent::processAttackPlayer()
{
	// ���m��player�̎擾
	PlayerActor* player = static_cast<GamePlay*>(mOwner->getSequence())->getPlayer();

	if (player->getHpComp()->isInvincible()) {
		return;
	}
	// �_���[�W�^����
	if (CheckCollisionRecs(player->getRectangle(), mCurInfo->colRect)) {
		mHitActors.push_back(player);
		Vector2 attackerPos = mOwner->getPosition();
		Vector2 targetPos = player->getPosition();
		Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));

		float flyspeed = 0.2f;
		direction.y -= flyspeed;
		direction = Vector2Normalize(direction);
		KnockbackInfo info;
		info.target = player;
		info.timer = 0.2f;
		float speed = 150.0f;
		info.velocity = Vector2Scale(direction, speed);

		mKnockbackTargets.push_back(info);

		player->getHpComp()->TakeDamage(mCurInfo->damage);
	}
}
