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
	// 첫 실행일 경우, field를 가져옴
	if (m_Field == nullptr)
		m_Field = _EnemyPair.pEnemyScript->GetField();

	// 3개의 enemy에게 TakeDamage와 애니메이션 재생 함수를 호출해야 한다.
	// 어떻게 해야 할까?
	// 1. 진행도를 가져오기
	// 2. 몬스터 벡터에서, 가장 뒤 쪽 오브젝트와 비교하며 MoveProgress가 같은 EnemyScript를 찾기
	// 3. enemy 3개에 데미지 주고 애니메이션 재생


	list<ENEMY_PAIR>& EnemyList = m_Field->GetEnemyList();
	auto rit = EnemyList.rbegin();

	// 현재 enemy 찾기
	float CurProgress = _EnemyPair.pEnemyScript->GetMoveProgress();

	for (; rit != EnemyList.rend(); ++rit)
	{
		if (rit->pEnemyScript->GetMoveProgress() == CurProgress)
			break;
	}

	// 예외
	if (rit == EnemyList.rend())
		return;

	// 데미지, 애니메이션
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
