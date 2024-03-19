#pragma once
#include <Engine/CScript.h>
#include "CExternScript.h"
#include "CFieldScript.h"

// 필드 위에 존재할 주사위 게임 오브젝트에 부착될 스크립트.
class CDiceScript
	:public CScript
{
private:
	// if DICE == NONE일 경우, 현재 필드 칸 위에 주사위가 존재하지 않음. 이 경우 아무 행동도 하지 않아요.
	// 음...아니면 bool 값으로 주사위 자리 활성화 여부를 둘까...?

	CFieldScript* m_OwnerField;


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


public:
	DICE GetDice() const { return Dice; }


	float GetFinalDamage() { return m_finalDamage; }
	float GetFinalAttackSpeed() { return m_finalAttackSpeed; }

	void SetDiceWithInfo(DICE _Dice);


	void SetDiceXY(UINT _DiceRow, UINT _DiceCol);

	void SetField(CFieldScript* _field) { m_OwnerField = _field; }

	virtual void begin() override;
public:
	virtual void SaveToFile(FILE* _File) {  }
	virtual void LoadFromFile(FILE* _File) {  }

public:
	
	CLONE(CDiceScript);

	CDiceScript();
	~CDiceScript();

};

