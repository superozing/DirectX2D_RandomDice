#include "pch.h"
#include "CDiceScale.h"

CDiceScale::CDiceScale()
	: CScript(SCRIPT_TYPE::DICESCALE)
{
}

CDiceScale::CDiceScale(const CDiceScale& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CDiceScale::~CDiceScale()
{
}

void CDiceScale::Attack()
{
	// ��.. ��� ���⼭ ���ο� ����ü�� ���� �Ǵ°�����
	// �������� ����ϴ°� ���� �� ȿ�������� ������?
	// �ϴ��� �������� ������� ����.

	m_pTargetEnemy;





}

void CDiceScale::ClearDiceScale()
{
	for (auto& it : m_ProjectileList)
		it.StopProjectile();



}
