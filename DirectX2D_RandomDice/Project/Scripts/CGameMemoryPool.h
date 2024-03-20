#pragma once
#include <vector>

using std::vector;

// ���� ��ü�� �����ϱ� ���� �޸� Ǯ Ŭ����
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

