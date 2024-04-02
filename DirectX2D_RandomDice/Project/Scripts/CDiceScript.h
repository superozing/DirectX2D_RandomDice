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
    // 아냐. 이런 식으로 하면 안된다.
    // 너무 종류가 많아진다. 절대 이런 식으로 구현하지 않았을꺼야.
    // 공격 스크립트, 버프 스크립트, 합쳐질 때 스크립트 이렇게 세 개를 만들자.
    // 그리고 파생시키는 것이 좋아보인다.
    class CDiceAttackScript* pAttack;
    class CDiceBuffScript* pBuff;
    class CDiceMergeScript* pMerge;
};

// 필드 위에 존재할 주사위 게임 오브젝트에 부착될 스크립트.
class CDiceScript
	:public CScript
{
private:
    // 필드
	CFieldScript*               m_OwnerField;

    // 주사위 정보
	DICE		                m_Dice;			// 주사위 종류
	DICE_INFO	                m_Info;			// 주사위 종류에 따른 주사위 정보
    Vec3                        m_DiceColor;    // 주사위 눈금 색상

    // 눈금
	UINT		                m_DiceScale;	// 주사위의 눈금 수 (등급)
	UINT		                m_CurDiceScaleIdx; // 현재 어떤 눈금에게 attack() 을 호출할 지 인덱스 값
	vector<CDiceScale*>         m_VecDiceScale;	
    float                       m_AttackTimer;

    // 주사위 오브젝트의 위치
	UINT		                m_DiceRow;
	UINT		                m_DiceCol;

    // 생성 효과
    CParticleSystem*            m_SpawnParticle;
    tParticleModule             tSpawnModule;
    Vec3                        m_vSrcScale;
    float                       m_fScaleSize;
    bool                        m_IsGrowing;

    // 레벨업 효과
    CParticleSystem*            m_LevelUpParticle;
    tParticleModule             tLevelUpModule;
    float                       m_LevelUpParticleTimer;


    // 적용 데미지와 공격 속도
	bool		                m_Buff[2];		// 이 주사위가 받고 있는 버프


	float		                m_finalDamage;		// 버프, 디버프를 포함한 데미지
	float		                m_finalAttackSpeed; // 버프, 디버프를 포함한 공격속도
    


public:
    // 전역 세팅 함수
    static void InitDice();

    static vector<wstring> DicePath;
    static void InitDicePath();
    static wstring GetDicePath(DICE _Dice) { return DicePath[(UINT)_Dice]; }

    static vector<Vec3> DiceColor;
    static void InitDiceColor();
    static Vec3 GetDiceColor(DICE _Dice) { return DiceColor[(UINT)_Dice]; }


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

