#pragma once
#include <vector>

using std::vector;

// 게임 객체를 관리하기 위한 메모리 풀 클래스
class CGameMemoryPool 
{
private:
    CGameObject* prototype;
    vector<CGameObject*> pool;

public:
    CGameMemoryPool();
    ~CGameMemoryPool();

    void SetObject(CGameObject* obj);
    CGameObject* Allocate();
    void Deallocate(CGameObject* obj);
};

