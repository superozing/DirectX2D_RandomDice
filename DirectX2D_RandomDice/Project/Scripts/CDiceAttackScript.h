#pragma once
#include <Engine/CScript.h>

enum class ATTACK_PRIORITY // 주사위의 공격 우선 순위
{
    BUFF,		 // 버프 주사위 - 공격을 하지 않음
    FRONT,		 // 앞 쪽 적 우선 공격
    HIGH_HEALTH, // 높은 체력의 적 우선 공격
    RANDOM,      // 무작위 적 공격
    RANGE_ATTACK,// 무작위 범위 공격
    END,
};

// 주사위 공격 정보 스크립트
class CDiceAttackScript :
    public CScript
{
private:
    // 자.. 어떤 정보가 필요할까요??


    /// 공격에 필요한 정보
    // 공격 목표
    ATTACK_PRIORITY     m_AttackPriority; // 여기서 설정한 공격 우선 순위에 따라서, 몬스터 리스트에서 뽑아놓은 몬스터에게 Attack를 호출해주기만 하면 된다.

    // 공격력과 공격 속도
    float               m_Damage;
    float               m_AttackSpeed;



    /// 눈금
    // 눈금 개수
    UINT                m_DiceScale;

    // 현재 공격을 실행시킬 눈금의 인덱스
    UINT                m_CurAttackDiceScaleIdx;

    // 눈금 벡터
    // 자... 드디어 파티클을 볼 때가 왔다.
    // 으아아아아...




public:


private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CDiceAttackScript);
    CDiceAttackScript();
    CDiceAttackScript(const CDiceAttackScript& _Origin);
    ~CDiceAttackScript();
};

