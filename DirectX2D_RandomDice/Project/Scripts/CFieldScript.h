#pragma once
#include <Engine\CScript.h>

// �ʵ��� �ֻ��� ��ġ�� �ֻ��� �Լ� ȣ���� ������ �ʵ� ��ũ��Ʈ
class CFieldScript :
    public CScript
{
private:
    class CGameObject* m_DiceField[5][3]; // CDiceScript�� ���� CGameObject�� 2���� �迭




public:
    virtual void SaveToFile(FILE* _File){}
    virtual void LoadFromFile(FILE* _File){}

public:
    CLONE_DISABLE(CFieldScript);
    CFieldScript();
    ~CFieldScript();

};
