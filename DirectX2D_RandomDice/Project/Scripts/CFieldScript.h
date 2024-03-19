#pragma once
#include <Engine\CScript.h>

// 필드의 주사위 배치와 주사위 함수 호출을 관리할 필드 스크립트
class CFieldScript :
    public CScript
{
private:
    class CDiceScript* m_DiceField[5][3]; // CDiceScript를 가진 CGameObject의 2차원 배열

    virtual void begin() override;


public:
    virtual void SaveToFile(FILE* _File){}
    virtual void LoadFromFile(FILE* _File){}

public:
    CLONE_DISABLE(CFieldScript);
    CFieldScript();
    ~CFieldScript();

};

