#pragma once
#include <Engine/CScript.h>
#include "CFieldScript.h"
#include "CDiceScale.h"

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
    class CDiceAttackScript* pAttack;
    class CDiceBuffScript* pBuff;
    class CDiceMergeScript* pMerge;
};

// �ʵ� ���� ������ �ֻ��� ���� ������Ʈ�� ������ ��ũ��Ʈ.
class CDiceScript
	:public CScript
{
private:
    // �ʵ�
	CFieldScript*               m_OwnerField;

    // �ֻ��� ����
	DICE		                m_Dice;			// �ֻ��� ����
	DICE_INFO	                m_Info;			// �ֻ��� ������ ���� �ֻ��� ����
    Vec3                        m_DiceColor;    // �ֻ��� ���� ����

    // ����
	UINT		                m_DiceScale;	// �ֻ����� ���� �� (���)
	UINT		                m_CurDiceScaleIdx; // ���� � ���ݿ��� attack() �� ȣ���� �� �ε��� ��
	vector<class CDiceScale*>         m_VecDiceScale;	
    float                       m_AttackTimer;

    // �ֻ��� ������Ʈ�� ��ġ
	UINT		                m_DiceRow;
	UINT		                m_DiceCol;

    // ���� ȿ��
    CParticleSystem*            m_SpawnParticle;
    tParticleModule             tSpawnModule;
    Vec3                        m_vSrcScale;
    float                       m_fScaleSize;
    bool                        m_IsGrowing;

    // ������ ȿ��
    CParticleSystem*            m_LevelUpParticle;
    tParticleModule             tLevelUpModule;
    float                       m_LevelUpParticleTimer;


    // ���� �������� ���� �ӵ�
	bool		                m_Buff[2];		// �� �ֻ����� �ް� �ִ� ����


	float		                m_finalDamage;		// ����, ������� ������ ������
	float		                m_finalAttackSpeed; // ����, ������� ������ ���ݼӵ�
    


public: // ����
    static void InitDice();

    static vector<wstring> DicePath;
    static void InitDicePath();
    static wstring GetDicePath(DICE _Dice) { return DicePath[(UINT)_Dice]; }

    static vector<Vec3> DiceColor;
    static void InitDiceColor();
    static Vec3 GetDiceColor(DICE _Dice) { return DiceColor[(UINT)_Dice]; }

    static CDiceAttackScript* GetDiceAttackScript(DICE _Dice);

public:
	DICE GetDice() const { return m_Dice; }


    float GetFinalDamage() const { return m_finalDamage; }
    float GetFinalAttackSpeed() const { return m_finalAttackSpeed; }

	void SetDiceType(DICE _Dice, UINT _DiceScale = 1);
	void SetDiceInfo(DICE _Dice);


	void SetDiceXY(UINT _DiceRow, UINT _DiceCol);

	void SetField(CFieldScript* _field) { m_OwnerField = _field; }


    void PlayLevelUp();




public:
	virtual void begin() override;
	virtual void tick() override;

public:
	virtual void SaveToFile(FILE* _File) {  }
	virtual void LoadFromFile(FILE* _File) {  }

public:
	CLONE(CDiceScript);
	CDiceScript();
	~CDiceScript();

};

