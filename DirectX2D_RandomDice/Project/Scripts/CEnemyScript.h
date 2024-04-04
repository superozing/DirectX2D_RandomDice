#pragma once
#include <Engine\CScript.h>
#include <Engine\CFontMgr.h>

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

    Vec3            MonScale;               // 몬스터 크기
};

// Enemy는 메모리 풀을 사용해서 관리하는게 어떨까
class CEnemyScript :
    public CScript
{
private:

    // 필드
    class CFieldScript*       m_OwnerField;
    
    // 적 종류와 정보
    ENEMY_TYPE        m_EnemyType;
    ENEMY_INFO        m_EnemyInfo;

    // 현재 체력
    int                 m_CurHealth;
    
    // 진행도 (0 ~ 100)
    float               m_MoveProgress;
    
    // 파티클 시스템 컴포넌트
    CGameObject*        m_pParticleObject;
    CParticleSystem*    m_ParticleSystem;

    float               m_DeathParticleTimer;

    bool                EndDeathParticle;

    // 크기 조절
    Vec3                m_vScale;
    int                 m_Minus = 1;
    float               m_fSizeScale = 1;

    bool                m_IsDeadEnemy;


    FONTINFO                m_fInfo1;
    FONTINFO                m_fInfo2;


    FONTINFO                m_fDamage;
    float                   m_DamageFontSizeScale;


private:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    virtual void tick() override;
    virtual void begin() override;

    void SetDeadEnemy();
    bool IsDeadEnemy() const { return m_IsDeadEnemy; }

    float GetMoveProgress() const { return m_MoveProgress; }
    ENEMY_TYPE GetEnemyType() const { return m_EnemyType; }
    bool IsEndDeathParticle() const { return EndDeathParticle; }


    void SetField(CFieldScript* _Field) { m_OwnerField = _Field; }

    int GetEnemyHealth() const { return m_CurHealth; }
    void SetEnemyHealth(int _Health) { m_CurHealth = _Health; }
    
    void TakeDamage(int _Dmg) { m_CurHealth -= _Dmg; }

protected:
    virtual void SetEnemyType(ENEMY_TYPE _Enemytype);
    virtual void SetEnemyInfo(ENEMY_INFO _EnemyInfo);


public:
    CLONE(CEnemyScript);
    CEnemyScript();
    CEnemyScript(SCRIPT_TYPE _ScriptType);
    CEnemyScript(const CEnemyScript& _Origin);
    ~CEnemyScript();
};

