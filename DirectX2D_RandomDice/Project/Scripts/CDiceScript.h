#pragma once
#include <Engine/CScript.h>
#include "CExternScript.h"

enum class ATTACK_PRIORITY // �ֻ����� ���� �켱 ����
{
	BUFF,		 // ���� �ֻ��� - ������ ���� ����
	FRONT,		 // �� �� �� �켱 ����
	HIGH_HEALTH, // ���� ü���� �� �켱 ����
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
	ATTACK_PRIORITY AttackPriority; // ���� �켱 ����
	DICE_BUFF		Buff;			// �� ��° �ε����� ������ �ִ� �ֻ��� �ΰ�?

	float			Damage;			// ������ ���� �ֻ����� ���ݷ�
	float			AttackSpeed;	// ������ ���� �ֻ����� ���� �ӵ�

	Vec3			DiceColor;		// ���� ����
};


// �ʵ� ���� ������ �ֻ��� ���� ������Ʈ�� ������ ��ũ��Ʈ.
class CDiceScript
	:public CScript
{
private:
	// if DICE == NONE�� ���, ���� �ʵ� ĭ ���� �ֻ����� �������� ����. �� ��� �ƹ� �ൿ�� ���� �ʾƿ�.
	// ��...�ƴϸ� bool ������ �ֻ��� �ڸ� Ȱ��ȭ ���θ� �ѱ�...?

	DICE		Dice;			// �ֻ��� ����
	DICE_INFO	m_Info;			// �ֻ��� ������ ���� �ֻ��� ����

	UINT		m_DiceGrade;	// �ֻ����� ���� �� (���)
	UINT		m_CurMarkerIdx; // ���� � ���ݿ��� attack() �� ȣ���� �� �ε��� ��
	vector<int> m_VecMarker;	/*���߿� ���� ����ü�� ���� ����*/

	bool		m_Buff[2];		// �� �ֻ����� �ް� �ִ� ����

	float		m_finalDamage;		// ����, ������� ������ ������
	float		m_finalAttackSpeed; // ����, ������� ������ ���ݼӵ�


	UINT		m_DiceRow;
	UINT		m_DiceCol;


	// �ֻ����� ���� ���¸� ������ �����̱� �ѵ�............. ���� �����غ��� ���� ��� �ϳ� �ͳ׿�?
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

