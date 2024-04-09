#include "pch.h"
#include "CDiceScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CDefaultAttack.h"
#include "CBuffAttack.h"
#include "CWindAttack.h"
#include "CElectricAttack.h"

#include "CDefaultMerge.h"

#include "CDiceBuffScript.h"
#include "CDiceAttackScript.h"
#include "CDiceMergeScript.h"

#include "CDiceScale.h"
#include "CFieldScript.h"
#include "CUIScript.h"


// 정의
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
	// 레벨에 소속되지 않은 전역 오브젝트와 스크립트를 삭제
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

	// 만약 공격 시간이 왔을 경우
	if (m_AttackTimer > (1.f / m_finalAttackSpeed) / m_DiceScale)
	{
		
		if (!m_VecDiceScale.empty())
		{
			// 현재 공격 차례인 눈금에게 Attack()을 호출하기
			m_VecDiceScale[m_CurDiceScaleIdx]->Attack();

			// 다음 인덱스로 옮기기
			++m_CurDiceScaleIdx;

			// 모든 눈금이 공격을 했다면, 다시 첫 눈금부터 공격
			if (m_DiceScale <= m_CurDiceScaleIdx)
				m_CurDiceScaleIdx = 0;
		}
		// 타이머 초기화
		m_AttackTimer -= (1.f / m_finalAttackSpeed) / m_DiceScale;
	}

}


void CDiceScript::SetDiceType(DICE _Dice, UINT _DiceScale)
{
	m_Dice = _Dice;
	m_DiceScale = _DiceScale;

	// 주사위 정보 클리어 (눈금, 주사위 능력 스크립트, 크기 배율, 파티클 등)
	ClearDice();

	// 세팅된 주사위가 없을 경우 return
	if (m_Dice == DICE::NONE) 
		return;

	// 주사위 공격 스크립트 부착
	m_Info.pAttack = CDiceScript::GetDiceAttackScript(m_Dice);

	// 주사위 병합 스크립트 부착
	m_Info.pMerge = CDiceScript::GetDiceMergeScript(m_Dice, this);

	// 주사위 색상
	m_DiceColor = CDiceScript::GetDiceColor(_Dice);

	// 주사위 눈금 생성
	SetDiceScale();

	// 파티클 색상 설정
	tSpawnModule.vSpawnColor = Vec4(m_DiceColor / 255.f, 1.f);
	tLevelUpModule.vSpawnColor = Vec4(m_DiceColor / 255.f, 1.f);

	m_SpawnParticle->SetParticleModule(tSpawnModule);
	m_LevelUpParticle->SetParticleModule(tLevelUpModule);

	// 생성 파티클, Scale 증가 효과
	PlaySpawnEffect();

	// 텍스쳐 바인딩
	OBJECT->GetRenderComponent()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, TEX_LOAD(CDiceScript::GetDicePath(m_Dice)));
}

void CDiceScript::ClearDice()
{
	// 모든 눈금 오브젝트 제거
	for (UINT i = 0; i < m_VecDiceScale.size(); ++i)
	{
		GamePlayStatic::DestroyGameObject(m_VecDiceScale[i]->GetOwner());
	}

	// 눈금 벡터 클리어
	m_VecDiceScale.clear();
	m_CurDiceScaleIdx = 0;

	// DiceInfo Clear
	if (m_Info.pAttack != nullptr)
		delete m_Info.pAttack;

	if (m_Info.pBuff != nullptr)
		delete m_Info.pBuff;

	if (m_Info.pMerge != nullptr)
		delete m_Info.pMerge;

	m_Info = DICE_INFO();

	m_fScaleSize = 0.f;
	m_IsGrowing = false;
}

void CDiceScript::SetDiceScale()
{
	// 새로 눈금 수에 따른 눈금 할당하기
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

	// 눈금 수에 따른 눈금들의 위치 세팅
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

			// 별 모양 텍스쳐로 변경
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
	if (GetDice() == DICE::NONE)
		return;

	// 필드에게서 현재 포커싱 된 주사위가 있는지 가져온다.
	CDiceScript* pFocusDice = m_OwnerField->GetFocusDice();

	// 포커싱 된 주사위가 없을 경우
	if (pFocusDice == nullptr)
	{
		m_OwnerField->SetFocusDice(this); // 자신을 포커싱 주사위로 세팅
		return;
	}

	// 포커싱 된 주사위가 자기 자신일 경우 (자신을 한 번 더 눌렀을 경우)
	if (pFocusDice == this)
	{
		m_OwnerField->SetFocusDice(nullptr); // 포커싱을 해제
		return;
	}

	// m_Info에서 주사위 병합 스크립트 가져오기 
	CDiceMergeScript* pMerge = m_Info.pMerge;

	// 만약 합칠 수 있을 경우 -> MergeDice 호출
	if (IsMergeAbleDice())
		pMerge->MergeDice(pFocusDice);

	m_OwnerField->SetFocusDice(nullptr); // 포커싱을 해제

}

void CDiceScript::PlayLevelUp()
{
	// LevelUp 텍스트 이미지 띄우도록 해주어야 하는데.
	m_LevelUpParticle->SetActivate(true);
	m_LevelUpParticleTimer = 0.15f;

	m_fScaleSize = 0.f;
	m_IsGrowing = true;
}


bool CDiceScript::IsMergeAbleDice()
{
	if (m_Info.pMerge)
		return m_Info.pMerge->IsMergeAble(m_OwnerField->GetFocusDice());
	else
		return false;
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

CDiceMergeScript* CDiceScript::GetDiceMergeScript(DICE _Dice, CDiceScript* _Inst)
{
	CDiceMergeScript* pMerge = nullptr;

	switch (_Dice)
	{
	case DICE::NONE:
		break;
	case DICE::FIRE:
		pMerge = new CDefaultMerge;
		break;
	case DICE::ELECTRIC:
		pMerge = new CDefaultMerge;
		break;
	case DICE::WIND:
		pMerge = new CDefaultMerge;
		break;
	case DICE::POISON:
		pMerge = new CDefaultMerge;
		break;
	case DICE::ICE:
		pMerge = new CDefaultMerge;
		break;
	case DICE::IRON:
		pMerge = new CDefaultMerge;
		break;
	case DICE::BROKEN:
		pMerge = new CDefaultMerge;
		break;
	case DICE::GAMBLE:
		pMerge = new CDefaultMerge;
		break;
	case DICE::LOCK:
		pMerge = new CDefaultMerge;
		break;
	case DICE::MINE:
		pMerge = new CDefaultMerge;
		break;
	case DICE::LIGHT:
		pMerge = new CDefaultMerge;
		break;
	case DICE::THORN:
		pMerge = new CDefaultMerge;
		break;
	case DICE::CRACK:
		pMerge = new CDefaultMerge;
		break;
	case DICE::CRITICAL:
		pMerge = new CDefaultMerge;
		break;
	case DICE::ENERGY:
		pMerge = new CDefaultMerge;
		break;
	case DICE::SACRIFICE:
		pMerge = new CDefaultMerge;
		break;
	case DICE::ARROW:
		pMerge = new CDefaultMerge;
		break;
	case DICE::MIMIC:
		pMerge = new CDefaultMerge;
		break;
	case DICE::END:
		break;
	default:
		break;
	}

	// 소유주를 인자로 받아온 객체로 설정
	pMerge->SetOwnerDice(_Inst);

	return pMerge;
}

CGameObject* CDiceScript::GetDefaultProjectileAnimObj()
{
	// 첫 실행 시 프리팹 로드
	if (m_ProjectileAnimator2DPref == nullptr)
	{
		m_ProjectileAnimator2DPref = CAssetMgr::GetInst()->Load<CPrefab>(
			L"prefab\\ProjectileAnimator2DPrefab.pref", L"prefab\\ProjectileAnimator2DPrefab.pref");
	}

	return m_ProjectileAnimator2DPref->Instantiate();
}

#undef OBJECT