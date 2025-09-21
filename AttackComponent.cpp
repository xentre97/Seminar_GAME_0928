#include "AttackComponent.h"

#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"
#include "Boss.h"

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
	}
}

void AttackComponent::startAttack(AttackInfo* info)
{
	mActive = true;
	mTimer = 0.0f;
	mCurInfo = info; // ���݂̍U����ݒ�
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

		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			if (auto boss = dynamic_cast<Boss*>(enemy)) {
				// ★ Boss だけはガードを考慮したダメージ適用
				boss->ApplyDamage(mCurInfo->damage, mCurInfo->tag);
				// （倒れ判定はBoss側/HP側で処理。ここでsetStateは不要）
			} else {
				if (enemy->getHpComp()->TakeDamage(mCurInfo->damage)) {
					enemy->setState(Actor::Edead);
					mActive = false;
				}
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

	// �_���[�W�^����
	if (CheckCollisionRecs(player->getRectangle(), mCurInfo->colRect)) {
		player->getHpComp()->TakeDamage(mCurInfo->damage);
	}
}
