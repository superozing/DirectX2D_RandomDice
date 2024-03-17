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
	DICE			Dice;			// �ֻ��� ����
	
	ATTACK_PRIORITY AttackPriority; // ���� �켱 ����
	DICE_BUFF		Buff;			// �� ��° �ε����� ������ �ִ� �ֻ��� �ΰ�?

	float			Damage;			// ������ ���� �ֻ����� ���ݷ�
	float			AttackSpeed;	// ������ ���� �ֻ����� ���� �ӵ�

	Vec3			DiceColor;		// ���� ����
};


class CDiceScript
	:public CScript
{
private:
	DICE_INFO	m_Info;

	UINT		m_DiceGrade;	// �ֻ����� ���� �� (���)
	UINT		m_CurMarkerIdx; // ���� � ���ݿ��� attack() �� ȣ���� �� �ε��� ��
	vector<int> m_VecMarker;	/*���߿� ���� ����ü�� ���� ����*/

	bool		m_Buff[2];		// �� �ֻ����� �ް� �ִ� ����

	float		m_finalDamage;		// ����, ������� ������ ������
	float		m_finalAttackSpeed; // ����, ������� ������ ���ݼӵ�


	UINT		m_DiceLow;
	UINT		m_DiceCol;


	// �ֻ����� ���� ���¸� ������ �����̱� �ѵ�............. ���� �����غ��� ���� ��� �ϳ� �ͳ׿�?
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

