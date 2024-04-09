#include "pch.h"
#include "CDiceScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CDefaultAttack.h"
#include "CBuffAttack.h"
#include "CWindAttack.h"
#include "CElectricAttack.h"

#include "CDiceBuffScript.h"
#include "CDiceAttackScript.h"
#include "CDiceMergeScript.h"

#include "CDiceScale.h"
#include "CFieldScript.h"
#include "CUIScript.h"


// ����
vector<wstring> CDiceScript::DicePath;
vector<Vec3>	CDiceScript::DiceColor;
Ptr<CPrefab>	CDiceScript::m_ProjectileAnimator2DPref = nullptr;

CDiceScript::CDiceScript()
	: CScript(DICESCRIPT)
	, m_fScaleSize(1.f)
	, m_DiceScale(0)
{
}

CDiceScript::~CDiceScript()
{
	// ������ �Ҽӵ��� ���� ���� ������Ʈ�� ��ũ��Ʈ�� ����
	if (m_Info.pAttack != nullptr)
		delete m_Info.pAttack;
	if (m_Info.pBuff != nullptr)
		delete m_Info.pBuff;
	if (m_Info.pMerge != nullptr)
		delete m_Info.pMerge;
}

#define OBJECT GetOwner()

void CDiceScript::tick()
{
	//==============
	// Set Transform
	//==============

	if (m_LevelUpParticleTimer > 0.f)
	{
		m_LevelUpParticleTimer -= DT;
		if (m_LevelUpParticleTimer <= 0.f)
		{
			m_LevelUpParticleTimer = 0.f;
			m_LevelUpParticle->SetActivate(false);
		}
	}

	if (m_IsGrowing)
	{
		m_fScaleSize += 5.f * DT;
		if (m_fScaleSize > 0.5f)
			m_SpawnParticle->SetActivate(false);

		if (m_fScaleSize > 1.f)
		{
			m_fScaleSize = 1.f;
			m_IsGrowing = false;
		}
	}

	OBJECT->Transform()->SetRelativeScale(m_vSrcScale * m_fScaleSize);

	OBJECT->GetRenderComponent()->GetDynamicMaterial()
		->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fScaleSize);



	//============
	// Dice Attack
	//============

	if (DICE::NONE != m_Dice)
	{
		m_AttackTimer += DT;
		m_finalAttackSpeed = m_Info.pAttack->GetAttackSpeed();
	}

	// ���� ���� �ð��� ���� ���
	if (m_AttackTimer > (1.f / m_finalAttackSpeed) / m_DiceScale)
	{
		
		if (!m_VecDiceScale.empty())
		{
			// ���� ���� ������ ���ݿ��� Attack()�� ȣ���ϱ�
			m_VecDiceScale[m_CurDiceScaleIdx]->Attack();

			// ���� �ε����� �ű��
			++m_CurDiceScaleIdx;

			// ��� ������ ������ �ߴٸ�, �ٽ� ù ���ݺ��� ����
			if (m_DiceScale <= m_CurDiceScaleIdx)
				m_CurDiceScaleIdx = 0;
		}
		// Ÿ�̸� �ʱ�ȭ
		m_AttackTimer -= (1.f / m_finalAttackSpeed) / m_DiceScale;
	}

}


void CDiceScript::SetDiceType(DICE _Dice, UINT _DiceScale)
{
	m_Dice = _Dice;
	m_DiceScale = _DiceScale;

	// ��� ���� ������Ʈ ����
	for (UINT i = 0; i < m_VecDiceScale.size(); ++i)
	{
		GamePlayStatic::DestroyGameObject(m_VecDiceScale[i]->GetOwner());
	}

	// ���� ���� Ŭ����
	m_VecDiceScale.clear();

	// ���� ��ƼŬ ȣ�� ���� ũ�⸦ 0���� ����
	m_fScaleSize = 0.f;
	m_IsGrowing = true;

	if (m_Dice == DICE::NONE) // ���õ� �ֻ����� ���� ��� return
	{
		m_IsGrowing = false;
		return;
	}

	// DiceInfo Clear
	if (m_Info.pAttack != nullptr)
	{
		delete m_Info.pAttack;
		m_Info.pAttack = nullptr;
	}
	if (m_Info.pBuff != nullptr)
	{
		delete m_Info.pBuff;
		m_Info.pBuff = nullptr;
	}
	if (m_Info.pMerge != nullptr)
	{
		delete m_Info.pMerge;
		m_Info.pMerge = nullptr;
	}

	//================
	// �ֻ��� ���� ����
	//================

	// �ֻ��� ���� ��ũ��Ʈ ����
	m_Info.pAttack = CDiceScript::GetDiceAttackScript(m_Dice);

	// �ֻ��� ����
	m_DiceColor = CDiceScript::GetDiceColor(_Dice);

	// �ֻ��� ���� ����
	SetDiceScale();

	tSpawnModule.vSpawnColor = Vec4(m_DiceColor / 255.f, 1.f);
	tLevelUpModule.vSpawnColor = Vec4(m_DiceColor / 255.f, 1.f);

	m_SpawnParticle->SetParticleModule(tSpawnModule);
	m_LevelUpParticle->SetParticleModule(tLevelUpModule);

	m_SpawnParticle->SetActivate(true);

	m_IsGrowing = true;

	wstring wstrPath = CDiceScript::GetDicePath(m_Dice);

	OBJECT->GetRenderComponent()->GetDynamicMaterial()
		->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));
}

void CDiceScript::SetDiceScale()
{
	// ���� ���� ���� ���� ���� �Ҵ��ϱ�
	m_VecDiceScale.resize(m_DiceScale);

	for (UINT i = 0; i < m_DiceScale; ++i)
	{
		CGameObject* pObj = new CGameObject;
		pObj->AddComponent(new CTransform);

		// MeshRender Set
		pObj->AddComponent(new CMeshRender);
		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
		pObj->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		pObj->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_DiceColor, 1.f));
		pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
			, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Dice\\dice_eye.png", L"texture\\Dice\\dice_eye.png"));

		m_VecDiceScale[i] = new CDiceScale;
		pObj->AddComponent(m_VecDiceScale[i]);

		m_VecDiceScale[i]->SetDiceScript(this);
		m_VecDiceScale[i]->SetField(m_OwnerField);
		m_VecDiceScale[i]->SetDiceColor(m_DiceColor);
		m_VecDiceScale[i]->SetSrcScale(Vec3(10.f, 10.f, 1.f));
		m_VecDiceScale[i]->SetDiceAttackScript(m_Info.pAttack);

		OBJECT->AddChild(pObj);
	}

	// ���� ���� ���� ���ݵ��� ��ġ ����
	switch (m_DiceScale)
	{
	case 1:
	{
		m_VecDiceScale[0]->GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
		break;
	}
	case 2:
	{
		m_VecDiceScale[0]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, 10.f, -10.f));
		m_VecDiceScale[1]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, -10.f, -10.f));

		break;
	}
	case 3:
	{
		m_VecDiceScale[0]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, 10.f, -10.f));
		m_VecDiceScale[1]->GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
		m_VecDiceScale[2]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, -10.f, -10.f));
		break;
	}
	case 4:
	{
		m_VecDiceScale[0]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, 10.f, -10.f));
		m_VecDiceScale[1]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, 10.f, -10.f));
		m_VecDiceScale[2]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, -10.f, -10.f));
		m_VecDiceScale[3]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, -10.f, -10.f));

		break;
	}
	case 5:
	{
		m_VecDiceScale[0]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, 10.f, -10.f));
		m_VecDiceScale[1]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, 10.f, -10.f));
		m_VecDiceScale[2]->GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
		m_VecDiceScale[3]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, -10.f, -10.f));
		m_VecDiceScale[4]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, -10.f, -10.f));
		break;
	}
	case 6:
	{
		m_VecDiceScale[0]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, 13.f, -10.f));
		m_VecDiceScale[1]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, 13.f, -10.f));
		m_VecDiceScale[2]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, 0.f, -10.f));
		m_VecDiceScale[3]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, 0.f, -10.f));
		m_VecDiceScale[4]->GetOwner()->Transform()->SetRelativePos(Vec3(-10.f, -13.f, -10.f));
		m_VecDiceScale[5]->GetOwner()->Transform()->SetRelativePos(Vec3(10.f, -13.f, -10.f));
		break;
	}
	case 7:
	{
		for (UINT i = 0; i < m_DiceScale; ++i)
		{
			m_VecDiceScale[i]->GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
			m_VecDiceScale[i]->SetSrcScale(Vec3(18.f, 18.f, 1.f));

			// �� ��� �ؽ��ķ� ����
			m_VecDiceScale[i]->GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
				, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Dice\\dice_7.png", L"texture\\Dice\\dice_7.png"));
		}

		break;
	}
	default:
		break;
	}
}

void CDiceScript::DiceClickedDelegateFunc()
{
	// �ʵ忡�Լ� ���� ��Ŀ�� �� �ֻ����� �ִ��� �����´�.
	CDiceScript* pFocusDice = m_OwnerField->GetFocusDice();

	// ��Ŀ�� �� �ֻ����� ���� ���
	if (pFocusDice == nullptr)
	{
		m_OwnerField->SetFocusDice(this); // �ڽ��� ��Ŀ�� �ֻ����� ����
		return;
	}

	// ��Ŀ�� �� �ֻ����� �ڱ� �ڽ��� ��� (�ڽ��� �� �� �� ������ ���)
	if (pFocusDice == this)
	{
		m_OwnerField->SetFocusDice(nullptr); // ��Ŀ���� ����
		return;
	}

	// ��Ŀ�� �� �ֻ����� �ٸ� �ֻ����� ��� ������ �� �ִ��� �Ǵ� (���߿� �Ǵ��ϴ� �Լ��� �߰��ؾ� �Ѵ�.)
	if (pFocusDice->GetDice() == GetDice() && pFocusDice->GetDiceScale() == GetDiceScale())// ������ �� �ִ� ���
	{
		// ��Ŀ�� �ֻ��� ����
		pFocusDice->SetDiceType(DICE::NONE);
		
		// ���� �ֻ����� ���� ����
		SetDiceType(m_OwnerField->GetRandomDeckDiceType(), GetDiceScale() + 1);
	}
	
	m_OwnerField->SetFocusDice(nullptr); // ��Ŀ���� ����

}

void CDiceScript::PlayLevelUp()
{
	// LevelUp �ؽ�Ʈ �̹��� ��쵵�� ���־�� �ϴµ�.
	m_LevelUpParticle->SetActivate(true);
	m_LevelUpParticleTimer = 0.15f;

	m_fScaleSize = 0.f;
	m_IsGrowing = true;
}


//=============
// Static Func
//=============

CDiceAttackScript* CDiceScript::GetDiceAttackScript(DICE _Dice)
{
	CDiceAttackScript* pAttack = nullptr;

	switch (_Dice)
	{
	case DICE::NONE:
		break;
	case DICE::FIRE:
		pAttack = new CDefaultAttack;
		break;
	case DICE::ELECTRIC:
		pAttack = new CElectricAttack;
		break;
	case DICE::WIND:
		pAttack = new CWindAttack;
		break;
	case DICE::POISON:
		pAttack = new CDefaultAttack;
		break;
	case DICE::ICE:
		pAttack = new CDefaultAttack;
		break;
	case DICE::IRON:
		pAttack = new CDefaultAttack;
		break;
	case DICE::BROKEN:
		pAttack = new CDefaultAttack;
		break;
	case DICE::GAMBLE:
		pAttack = new CDefaultAttack;
		break;
	case DICE::LOCK:
		pAttack = new CDefaultAttack;
		break;
	case DICE::MINE:
		pAttack = new CBuffAttack;
		break;
	case DICE::LIGHT:
		pAttack = new CBuffAttack;
		break;
	case DICE::THORN:
		pAttack = new CDefaultAttack;
		break;
	case DICE::CRACK:
		pAttack = new CDefaultAttack;
		break;
	case DICE::CRITICAL:
		pAttack = new CBuffAttack;
		break;
	case DICE::ENERGY:
		pAttack = new CDefaultAttack;
		break;
	case DICE::SACRIFICE:
		pAttack = new CDefaultAttack;
		break;
	case DICE::ARROW:
		pAttack = new CDefaultAttack;
		break;
	case DICE::MIMIC:
		pAttack = new CDefaultAttack;
		break;
	case DICE::END:
		break;
	default:
		break;
	}

	return pAttack;
}

CGameObject* CDiceScript::GetDefaultProjectileAnimObj()
{
	// ù ���� �� ������ �ε�
	if (m_ProjectileAnimator2DPref == nullptr)
	{
		m_ProjectileAnimator2DPref = CAssetMgr::GetInst()->Load<CPrefab>(
			L"prefab\\ProjectileAnimator2DPrefab.pref", L"prefab\\ProjectileAnimator2DPrefab.pref");
	}

	return m_ProjectileAnimator2DPref->Instantiate();
}

#undef OBJECT