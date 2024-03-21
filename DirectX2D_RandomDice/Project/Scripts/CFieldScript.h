#pragma once
#include <Engine\CScript.h>
#include "CGameMemoryPool.h"
#include "CEnemyScript.h"

// �ʵ��� �ֻ��� ��ġ�� �ֻ��� �Լ� ȣ���� ������ �ʵ� ��ũ��Ʈ
class CFieldScript :
    public CScript
{
private:
    class CDiceScript*  m_DiceField[5][3]; // CDiceScript�� ���� CGameObject�� 2���� �迭

    list<CGameObject*>  m_EnemyList;

    CGameMemoryPool     m_EnemyPool[(UINT)ENEMY_TYPE::END];

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    virtual void SaveToFile(FILE* _File){}
    virtual void LoadFromFile(FILE* _File){}

public:
    CLONE_DISABLE(CFieldScript);
    CFieldScript();
    ~CFieldScript();

};

