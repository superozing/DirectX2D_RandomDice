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
	ATTACK_PRIORITY AttackPriority; // 공격 우선 순위
	DICE_BUFF		Buff;			// 몇 번째 인덱스에 버프를 주는 주사위 인가?

	float			Damage;			// 종류에 따른 주사위의 공격력
	float			AttackSpeed;	// 종류에 따른 주사위의 공격 속도

	Vec3			DiceColor;		// 눈금 색상
};


// 필드 위에 존재할 주사위 게임 오브젝트에 부착될 스크립트.
class CDiceScript
	:public CScript
{
private:
	// if DICE == NONE일 경우, 현재 필드 칸 위에 주사위가 존재하지 않음. 이 경우 아무 행동도 하지 않아요.
	// 음...아니면 bool 값으로 주사위 자리 활성화 여부를 둘까...?

	DICE		Dice;			// 주사위 종류
	DICE_INFO	m_Info;			// 주사위 종류에 따른 주사위 정보

	UINT		m_DiceGrade;	// 주사위의 눈금 수 (등급)
	UINT		m_CurMarkerIdx; // 현재 어떤 눈금에게 attack() 을 호출할 지 인덱스 값
	vector<int> m_VecMarker;	/*나중에 눈금 구조체가 들어가는 벡터*/

	bool		m_Buff[2];		// 이 주사위가 받고 있는 버프

	float		m_finalDamage;		// 버프, 디버프를 포함한 데미지
	float		m_finalAttackSpeed; // 버프, 디버프를 포함한 공격속도


	UINT		m_DiceRow;
	UINT		m_DiceCol;


	// 주사위의 현재 상태를 저장할 예정이긴 한데............. 지금 생각해보니 굳이 써야 하나 싶네요?
	CStateMachine* m_StateMachine;


public:
	DICE GetDice() const { return Dice; }

	void SetDiceWithInfo(DICE _Dice) { Dice = _Dice; }


	void SetDiceRow(UINT _DiceRow) { m_DiceRow = _DiceRow; }
	void SetDiceCol(UINT _DiceCol) { m_DiceCol = _DiceCol; }


public:
	virtual void SaveToFile(FILE* _File) {  }
	virtual void LoadFromFile(FILE* _File) {  }

public:
	
	CLONE(CDiceScript);

	CDiceScript();
	~CDiceScript();

};

