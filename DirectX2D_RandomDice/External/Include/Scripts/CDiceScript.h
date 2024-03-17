#pragma once
#include <Engine/CScript.h>
#include "CExternScript.h"

enum class ATTACK_PRIORITY // 주사위의 공격 우선 순위
{
	BUFF,		 // 버프 주사위 - 공격을 하지 않음
	FRONT,		 // 앞 쪽 적 우선 공격
	HIGH_HEALTH, // 높은 체력의 적 우선 공격
	END,
};

enum class DICE_BUFF
{
	NONE,
	CRITICAL,
	ATTACK_SPEED,
};


struct DICE_INFO
{
	DICE			Dice;			// 주사위 종류
	
	ATTACK_PRIORITY AttackPriority; // 공격 우선 순위
	DICE_BUFF		Buff;			// 몇 번째 인덱스에 버프를 주는 주사위 인가?

	float			Damage;			// 종류에 따른 주사위의 공격력
	float			AttackSpeed;	// 종류에 따른 주사위의 공격 속도

	Vec3			DiceColor;		// 눈금 색상
};


class CDiceScript
	:public CScript
{
private:
	DICE_INFO	m_Info;

	UINT		m_DiceGrade;	// 주사위의 눈금 수 (등급)
	UINT		m_CurMarkerIdx; // 현재 어떤 눈금에게 attack() 을 호출할 지 인덱스 값
	vector<int> m_VecMarker;	/*나중에 눈금 구조체가 들어가는 벡터*/

	bool		m_Buff[2];		// 이 주사위가 받고 있는 버프

	float		m_finalDamage;		// 버프, 디버프를 포함한 데미지
	float		m_finalAttackSpeed; // 버프, 디버프를 포함한 공격속도


	UINT		m_DiceLow;
	UINT		m_DiceCol;


	// 주사위의 현재 상태를 저장할 예정이긴 한데............. 지금 생각해보니 굳이 써야 하나 싶네요?
	CStateMachine* m_StateMachine;


public:
	DICE_INFO GetDiceInfo() const { return m_Info; }



public:
	virtual void SaveToFile(FILE* _File) {  }
	virtual void LoadFromFile(FILE* _File) {  }

public:
	
	CLONE(CDiceScript);

	CDiceScript();
	~CDiceScript();

};

