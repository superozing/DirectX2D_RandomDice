#pragma once
#include <Engine/CScript.h>
#include "CExternScript.h"
#include "CFieldScript.h"

// �ʵ� ���� ������ �ֻ��� ���� ������Ʈ�� ������ ��ũ��Ʈ.
class CDiceScript
	:public CScript
{
private:
	// if DICE == NONE�� ���, ���� �ʵ� ĭ ���� �ֻ����� �������� ����. �� ��� �ƹ� �ൿ�� ���� �ʾƿ�.
	// ��...�ƴϸ� bool ������ �ֻ��� �ڸ� Ȱ��ȭ ���θ� �ѱ�...?

	CFieldScript* m_OwnerField;


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

