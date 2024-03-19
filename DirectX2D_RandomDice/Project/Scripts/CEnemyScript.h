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
};

// Enemy�� �޸� Ǯ�� ����ؼ� �����ϴ°� ���
class CEnemyScript :
    public CScript
{
private:

    // �ʵ�
    CFieldScript*       m_OwnerField;
    
    // �� ������ ����
    ENEMY_TYPE        m_EnemyType;
    ENEMY_INFO        m_EnemyInfo;

    // ���� ü��
    int                 m_CurHealth;
    
    // ���൵ (0 ~ 100)
    float               m_MoveProgress;
    
    // ��ƼŬ �ý��� ������Ʈ
    CParticleSystem*    m_ParticleSystem;


    // ������ ����Ʈ



private:

    ///////////////////////////////////////////////////////////// �ڽ� �ʿ��� �������̵��� �ݵ�� �ؾ� �ؿ�.
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

