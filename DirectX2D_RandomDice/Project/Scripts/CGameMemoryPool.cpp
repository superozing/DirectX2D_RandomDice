#include "pch.h"
#include "CGameMemoryPool.h"
#include <Engine\CGameObject.h>

CGameMemoryPool::CGameMemoryPool() 
    : prototype(nullptr) 
{

}

CGameMemoryPool::~CGameMemoryPool() 
{
    for (auto p : pool) 
        delete p;

    delete prototype;
}

void CGameMemoryPool::SetObject(CGameObject* obj) 
{
    delete prototype; // 이전 프로토타입 삭제
    prototype = obj;
}

CGameObject* CGameMemoryPool::Allocate() 
{
    if (prototype == nullptr) 
        return nullptr;

    if (pool.empty()) 
        return prototype->Clone();

    else 
    {
        CGameObject* obj = pool.back();
        pool.pop_back();
        return obj;
    }
}

void CGameMemoryPool::Deallocate(CGameObject* obj) 
{
    pool.push_back(obj);
}
