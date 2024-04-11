#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"

// ���� ��� �Ŵ���
// ��������� ���� ����, �� ������Ʈ�� ���� ��� ������ ��ũ��Ʈ�� �����ϰ� ������ ���
// ���������� ���� ��尡 ���۵� �� �ֵ��� ������.
class CPracticeModeMgr :
    public CScript
{
private:
    /// ���� �ʵ� ���� ������Ʈ
    // 1. �ʵ�
    class CFieldScript*         m_Field;

    // 2. �ֻ��� ��ȯ ��ư
    class CSummonDiceBtnScript* m_SummonBtn;

    // 3. SP ��
    class CSPBarScript*         m_SPBar;

    // 4. �÷��̾� HP
    class CPlayerHP*            m_HP;

public:
    CFieldScript* GetField()
    {
        return m_Field;
    }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(CPracticeModeMgr);
    CPracticeModeMgr();
    CPracticeModeMgr(const CPracticeModeMgr& _Origin);
    ~CPracticeModeMgr();
};

