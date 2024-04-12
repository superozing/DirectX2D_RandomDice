#include "pch.h"
#include "CWaveTimer.h"
#include "CPracticeModeMgr.h"

CWaveTimer::CWaveTimer()
	: CScript(WAVETIMER)
{
}

CWaveTimer::CWaveTimer(const CWaveTimer& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CWaveTimer::~CWaveTimer()
{
}

void CWaveTimer::SetBoss(BOSS_TYPE _Boss)
{
	m_CurBoss = _Boss;

	m_BossIcon->MeshRender()->GetDynamicMaterial()
		->SetTexParam(TEX_PARAM::TEX_0, m_BossImgArr[(UINT)m_CurBoss]);
}

#define OBJECT GetOwner()

void CWaveTimer::begin()
{
	assert(OBJECT);

	//=====================
	// 트랜스폼 컴포넌트 설정
	//=====================
	if (OBJECT->Transform() == nullptr)
		OBJECT->AddComponent(new CTransform);

	assert(OBJECT->Transform());
	
	Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	Transform()->SetRelativeScale(Vec3(88.f, 100.f, 1.f));

	//=====================
	// 매쉬렌더 컴포넌트 설정
	//=====================

	// 다른 렌더 컴포넌트를 가진 경우 Assert 처리
	assert(!(OBJECT->GetRenderComponent() != nullptr && OBJECT->MeshRender() == nullptr));

	if (OBJECT->MeshRender() == nullptr)
		OBJECT->AddComponent(new CMeshRender);

	MeshRender()->SetMesh(ASSET_MGR->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(ASSET_MGR->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, TEX_LOAD(L"texture\\Battle\\ingame_quest.png"));



	///

	m_BossImgArr[(UINT)BOSS_TYPE::SNAKE] = TEX_LOAD(L"texture\\Battle\\battle_boss_10.png");
	m_BossImgArr[(UINT)BOSS_TYPE::SILENCE] = TEX_LOAD(L"texture\\Battle\\battle_boss_20.png");
	m_BossImgArr[(UINT)BOSS_TYPE::RANDOM_KNIGHT] = TEX_LOAD(L"texture\\Battle\\battle_boss_30.png");

	m_BossIcon = new CGameObject;
	GetOwner()->AddChild(m_BossIcon);
	m_BossIcon->AddComponent(new CTransform);
	m_BossIcon->Transform()->SetRelativePos(Vec3(0.f, 8.f, -1.f));
	m_BossIcon->Transform()->SetRelativeScale(Vec3(65.f, 65.f, 1.f));

	m_BossIcon->AddComponent(new CMeshRender);
	m_BossIcon->MeshRender()->SetMesh(ASSET_MGR->FindAsset<CMesh>(L"RectMesh"));
	m_BossIcon->MeshRender()->SetMaterial(ASSET_MGR->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
	m_BossIcon->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_BossImgArr[(UINT)BOSS_TYPE::SNAKE]);


	m_WaveTimeFontInfo.Color = FONT_RGBA(0xff, 0xd4, 0, 255);
	m_WaveTimeFontInfo.fFontSize = 13.f;
	m_WaveTimeFontInfo.FontType = FONT_TYPE::MAPLE;
	m_WaveTimeFontInfo.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_WaveTimeFontInfo.IsWorldPosRender = true;
	m_WaveTimeFontInfo.fPosX = 0.f;
	m_WaveTimeFontInfo.fPosY = 62.f;

}

void CWaveTimer::tick()
{
	// 폰트 추가
	if (m_ModeMgr->IsBossAppears())
	{
		m_WaveTimeFontInfo.WStr = L"보스 출현";
	}
	else
	{
		wstring wstr = to_wstring((int)m_ModeMgr->GetCurWaveTime());

		if (wstr.length() == 1)
			m_WaveTimeFontInfo.WStr = L"00:0" + wstr;
		else
			m_WaveTimeFontInfo.WStr = L"00:" + wstr;

	}
	CFontMgr::GetInst()->AddRenderFont(m_WaveTimeFontInfo);


}
#undef OBJECT
