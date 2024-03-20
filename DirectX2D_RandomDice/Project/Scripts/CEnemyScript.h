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
    //CFieldScript*       m_OwnerField;
    
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

    // ������ ����Ʈ



private:

    ///////////////////////////////////////////////////////////// �ڽ� �ʿ��� �������̵��� �ݵ�� �ؾ� �ؿ�.
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    virtual void tick() override;
    virtual void begin() override;

    void PlayDeathParticle();

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

