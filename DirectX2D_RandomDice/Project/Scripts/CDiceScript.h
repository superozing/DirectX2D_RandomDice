#pragma once
#include <Engine/CScript.h>
#include "CFieldScript.h"

enum class DICE
{
    NONE,

    FIRE,
    ELECTRIC,
    WIND,
    POISON,
    ICE,

    IRON,
    BROKEN,
    GAMBLE,
    LOCK,
    MINE,

    LIGHT,
    THORN,
    CRACK,
    CRITICAL,
    ENERGY,

    SACRIFICE,
    ARROW,
    MIMIC,

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
    // �Ƴ�. �̷� ������ �ϸ� �ȵȴ�.
    // �ʹ� ������ ��������. ���� �̷� ������ �������� �ʾ�������.
    // ���� ��ũ��Ʈ, ���� ��ũ��Ʈ, ������ �� ��ũ��Ʈ �̷��� �� ���� ������.
    // �׸��� �Ļ���Ű�� ���� ���ƺ��δ�.
    class CDiceAttackScript* pAttack;
    class CDiceBuffScript* pBuff;
    class CDiceMergeScript* pMerge;
};

// �ʵ� ���� ������ �ֻ��� ���� ������Ʈ�� ������ ��ũ��Ʈ.
class CDiceScript
	:public CScript
{
private:
	// if DICE == NONE�� ���, ���� �ʵ� ĭ ���� �ֻ����� �������� ����. �� ��� �ƹ� �ൿ�� ���� �ʾƿ�.
	// ��...�ƴϸ� bool ������ �ֻ��� �ڸ� Ȱ��ȭ ���θ� �ѱ�...?


	CFieldScript* m_OwnerField;


	DICE		m_Dice;			// �ֻ��� ����
	DICE_INFO	m_Info;			// �ֻ��� ������ ���� �ֻ��� ����

	UINT		m_DiceScale;	// �ֻ����� ���� �� (���)
	UINT		m_CurMarkerIdx; // ���� � ���ݿ��� attack() �� ȣ���� �� �ε��� ��
	vector<int> m_VecMarker;	/*���߿� ���� ����ü�� ���� ����*/

	bool		m_Buff[2];		// �� �ֻ����� �ް� �ִ� ����




	float		m_finalDamage;		// ����, ������� ������ ������
	float		m_finalAttackSpeed; // ����, ������� ������ ���ݼӵ�
	



	UINT		m_DiceRow;
	UINT		m_DiceCol;


public:
    // ���� ���� �Լ�
    static vector<wstring> DicePath;
    static void InitDicePath();



	DICE GetDice() const { return m_Dice; }


	float GetFinalDamage() { return m_finalDamage; }
	float GetFinalAttackSpeed() { return m_finalAttackSpeed; }

	void SetDiceType(DICE _Dice);
	void SetDiceInfo(DICE _Dice);


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

