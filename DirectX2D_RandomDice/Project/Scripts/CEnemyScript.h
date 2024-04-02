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
    float               m_fScale = 1;



private:

    ///////////////////////////////////////////////////////////// �ڽ� �ʿ��� �������̵��� �ݵ�� �ؾ� �ؿ�.
    // ��� �Ⱦ��� ���ص� �Ǵ°� �ƴұ�?>
    // �´�. �Ⱦ��� ���ص� �ȴ�.
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    virtual void tick() override;
    virtual void begin() override;

    void PlayDeathParticle();

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

