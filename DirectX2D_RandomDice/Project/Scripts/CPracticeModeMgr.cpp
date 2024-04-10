#include "pch.h"
#include "CPracticeModeMgr.h"

CPracticeModeMgr::CPracticeModeMgr()
	: CScript(PRACTICEMODEMGR)
{
}

CPracticeModeMgr::CPracticeModeMgr(const CPracticeModeMgr& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CPracticeModeMgr::~CPracticeModeMgr()
{
}

#define OBJ GetOwner()

void CPracticeModeMgr::begin()
{
	if (Transform() == nullptr)
		OBJ->AddComponent(new CTransform);

	// 바 위치 설정
	Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	Transform()->SetRelativeScale(Vec3(540.f, 40.f, 1.f));

}

void CPracticeModeMgr::tick()
{
}

#undef OBJ
