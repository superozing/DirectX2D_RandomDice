#pragma once
#include <Engine\CScript.h>
#include "CFieldScript.h"

enum class ENEMY_TYPE
{
    DEFAULT,
    SPEED,
    BIG,
    BOSS,
    END
};

struct ENEMY_INFO
{
    Ptr<CTexture>   MonTex;                 // 몬스터 텍스쳐
    tParticleModule DeathParticleModule;    // 데스 파티클 모듈
    float           MoveSpeed;              // 초당 진행도(이동 속도)
};

// Enemy는 메모리 풀을 사용해서 관리하는게 어떨까
class CEnemyScript :
    public CScript
{
private:

    // 필드
    CFieldScript*       m_OwnerField;
    
    // 적 종류와 정보
    ENEMY_TYPE        m_EnemyType;
    ENEMY_INFO        m_EnemyInfo;

    // 현재 체력
    int                 m_CurHealth;
    
    // 진행도 (0 ~ 100)
    float               m_MoveProgress;
    
    // 파티클 시스템 컴포넌트
    CParticleSystem*    m_ParticleSystem;


    // 데미지 리스트



private:

    ///////////////////////////////////////////////////////////// 자식 쪽에서 오버라이딩을 반드시 해야 해요.
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    virtual void tick() override;
    virtual void begin() override;

protected:
    virtual void SetEnemyType(ENEMY_TYPE _Enemytype);
    virtual void SetEnemyInfo(ENEMY_INFO _EnemyInfo);


public:
    CLONE(CEnemyScript);
    CEnemyScript();
    CEnemyScript(const CEnemyScript& _Origin);
    ~CEnemyScript();
};

