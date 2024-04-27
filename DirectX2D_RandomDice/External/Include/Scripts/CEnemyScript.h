#pragma once
#include <Engine\CScript.h>
#include <Engine\CFontMgr.h>
#include "ScriptDefine.h"

struct ENEMY_INFO
{
    Ptr<CTexture>   MonTex;                 // ���� �ؽ���
    tParticleModule DeathParticleModule;    // ���� ��ƼŬ ���
    float           MoveSpeed;              // �ʴ� ���൵(�̵� �ӵ�)

    Vec3            MonScale;               // ���� ũ��
};

// Enemy�� �޸� Ǯ�� ����ؼ� �����ϴ°� ���
class CEnemyScript :
    public CScript
{
private:

    // �ʵ�
    class CFieldScript*       m_OwnerField;
    
    // �� ������ ����
    ENEMY_TYPE        m_EnemyType;
    ENEMY_INFO        m_EnemyInfo;

    // ���� ü��
    int                 m_CurHealth;
    
    // ���൵ (0 ~ 100)
    float               m_MoveProgress;
    
    // ��ƼŬ �ý��� ������Ʈ
    CGameObject*        m_pParticleObject;
    CParticleSystem*    m_ParticleSystem;

    float               m_DeathParticleTimer;

    bool                EndDeathParticle;

    // ũ�� ����
    Vec3                m_vScale;
    int                 m_Minus = 1;
    float               m_fSizeScale = 1;

    bool                m_IsDeadEnemy;

    // ��Ʈ
    FONTINFO                m_fInfo1;
    FONTINFO                m_fInfo2;


    FONTINFO                m_fDamage1;
    FONTINFO                m_fDamage2;
    float                   m_DamageFontSizeScale = 1.f;
    float                   m_SourceFontSize = 15.f;
    float                   m_DamageFontTimer;
    bool                    m_IsDamageFontRender = false;

    
    // �ִϸ��̼�
    CGameObject*            m_01_fireObject;       // ���� SpawnObject 
    CGameObject*            m_02_electricObject;   // Enemy ����

public: // ����
    static Ptr<CPrefab>            m_01_firePrefab;
    static CGameObject* Get01_firePrefabInstantiate();

    static Ptr<CPrefab>            m_02_electricPrefab;
    static CGameObject* Get02_electricPrefabInstantiate();

public:
    virtual void SetDeadEnemy();
    bool IsDeadEnemy() const { return m_IsDeadEnemy; }

    float GetMoveProgress() const { return m_MoveProgress; }
    ENEMY_TYPE GetEnemyType() const { return m_EnemyType; }
    bool IsEndDeathParticle() const { return EndDeathParticle; }

    void Play02_electricAnimation();
    CGameObject* Get02_electricObject() { return m_02_electricObject; }

    void SetField(CFieldScript* _Field) { m_OwnerField = _Field; }
    CFieldScript* GetField() { return m_OwnerField; }

    int GetEnemyHealth() const { return m_CurHealth; }
    void SetEnemyHealth(int _Health) { m_CurHealth = _Health; }
    
    void TakeDamage(int _Dmg, bool _IsCriticalAttack = false);


    void SetMoveSpeed(float _MoveSpeed) { m_EnemyInfo.MoveSpeed = _MoveSpeed; }
    float GetMoveSpeed() const { return m_EnemyInfo.MoveSpeed; }
    
public:
    virtual void BossSkill() {}
    virtual void EndBossSkill() {}


protected:
    virtual void SetEnemyType(ENEMY_TYPE _Enemytype);
    virtual void SetEnemyInfo(ENEMY_INFO _EnemyInfo);


public:
    virtual void begin() override;
    virtual void tick() override;


public:
    FSAVELOAD_DISABLE;
    CLONE(CEnemyScript);
    CEnemyScript();
    CEnemyScript(SCRIPT_TYPE _ScriptType);
    CEnemyScript(const CEnemyScript& _Origin);
    ~CEnemyScript();

    friend class CDiceAttackScript;
};

