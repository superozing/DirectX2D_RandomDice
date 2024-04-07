#include "pch.h"
#include "CElectricAttack.h"
#include "CFieldScript.h"
#include "CEnemyScript.h"

CElectricAttack::CElectricAttack()
	: CDiceAttackScript(ELECTRICATTACK)
{
	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CElectricAttack::ElectricAttackDelegateFunc);
	SetAttackDamage(80.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(2.f);
}

CElectricAttack::CElectricAttack(const CElectricAttack& _Origin)
	: CDiceAttackScript(_Origin.GetScriptType())
{
	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CElectricAttack::ElectricAttackDelegateFunc);
	SetAttackDamage(80.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(2.f);
}

CElectricAttack::~CElectricAttack()
{
}

void CElectricAttack::ElectricAttackDelegateFunc(ENEMY_PAIR _EnemyPair)
{
	// ù ������ ���, field�� ������
	if (m_Field == nullptr)
		m_Field = _EnemyPair.pEnemyScript->GetField();

	// 3���� enemy���� TakeDamage�� �ִϸ��̼� ��� �Լ��� ȣ���ؾ� �Ѵ�.
	// ��� �ؾ� �ұ�?
	// 1. ���൵�� ��������
	// 2. ���� ���Ϳ���, ���� �� �� ������Ʈ�� ���ϸ� MoveProgress�� ���� EnemyScript�� ã��
	// 3. enemy 3���� ������ �ְ� �ִϸ��̼� ���


	list<ENEMY_PAIR>& EnemyList = m_Field->GetEnemyList();
	auto rit = EnemyList.rbegin();

	// ���� enemy ã��
	float CurProgress = _EnemyPair.pEnemyScript->GetMoveProgress();

	for (; rit != EnemyList.rend(); ++rit)
	{
		if (rit->pEnemyScript->GetMoveProgress() == CurProgress)
			break;
	}

	// ����
	if (rit == EnemyList.rend())
		return;

	// ������, �ִϸ��̼�
	for (int i = 0; rit != EnemyList.rend() && i < 3; ++rit, ++i)
	{
		switch (i)
		{
		case 0:
			rit->pEnemyScript->TakeDamage(GetAttackDamage());
			break;
		case 1:
			rit->pEnemyScript->TakeDamage(GetAttackDamage() * 0.7);
			break;
		case 2:
			rit->pEnemyScript->TakeDamage(GetAttackDamage() * 0.3);
			break;
		default:
			break;
		}

		switch (rit->pEnemyScript->GetEnemyType())
		{
		case ENEMY_TYPE::DEFAULT:
			rit->pEnemyScript->Get02_electricObject()->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
			break;
		case ENEMY_TYPE::SPEED:
			rit->pEnemyScript->Get02_electricObject()->Transform()->SetRelativeScale(Vec3(70.f, 70.f, 1.f));
			break;
		case ENEMY_TYPE::BIG:
			rit->pEnemyScript->Get02_electricObject()->Transform()->SetRelativeScale(Vec3(130.f, 130.f, 1.f));
			break;
		case ENEMY_TYPE::BOSS:
			break;
		default:
			break;
		}

		rit->pEnemyScript->Play02_electricAnimation();

	}
}
