#pragma once
#include <Engine/CScript.h>
#include "CExternScript.h"

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


public:
	DICE GetDice() const { return Dice; }


	float GetFinalDamage() { return m_finalDamage; }
	float GetFinalAttackSpeed() { return m_finalAttackSpeed; }

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

