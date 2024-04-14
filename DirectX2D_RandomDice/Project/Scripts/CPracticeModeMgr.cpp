#include "pch.h"
#include "CPracticeModeMgr.h"

#include "CFieldScript.h"
#include "CSummonDiceBtnScript.h"
#include "CSPBarScript.h"
#include "CPlayerHP.h"
#include "CWaveTimer.h"

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

void CPracticeModeMgr::BossAppears()
{
	// ���� ����
	m_IsBossAppears = true;
	
	// ���̺긦 0���� ���߾ ���Ͱ� ��ȯ���� �ʵ��� ���ֱ�
	m_Field->SetCurWave(0);

	m_CurWaveTime = 0.f;
	m_Field->ClearEnemyList();
	m_Field->SpawnBoss(m_CurBoss);
}

void CPracticeModeMgr::WaveEnd()
{
	// ���� ���̺� ����
	++m_CurWave;
	m_Field->SetCurWave(m_CurWave);

	// ���̺� �ð� ����
	m_CurWaveTime = m_MaxWaveTime;

	// Wave Font Set
	m_WaveFontInfo.WStr = wstring(L"WAVE ") + to_wstring(m_CurWave);

	// Boss Set
	m_IsBossAppears = false;

	switch (m_CurBoss)
	{
	case BOSS_TYPE::SNAKE :
		m_CurBoss = BOSS_TYPE::SILENCE;
		break;
	case BOSS_TYPE::SILENCE :
		m_CurBoss = BOSS_TYPE::RANDOM_KNIGHT;
		break;
	case BOSS_TYPE::RANDOM_KNIGHT :
		m_CurBoss = BOSS_TYPE::SNAKE;
		break;
	}

	m_WaveTimer->SetBoss(m_CurBoss);
}

void CPracticeModeMgr::begin()
{
	OBJ->SetName(L"PracticeModeMgr");

	// Transform set
	if (Transform() == nullptr)
		OBJ->AddComponent(new CTransform);

	Transform()->SetRelativePos(Vec3(0.f, 80.f, 100.f));
	Transform()->SetRelativeScale(Vec3(540.f, 40.f, 1.f));


	// MeshRender set
	if (GetRenderComponent() == nullptr)
		OBJ->AddComponent(new CMeshRender);

	MeshRender()->SetMesh(ASSET_MGR->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(ASSET_MGR->FindAsset<CMaterial>(L"AlphaBlendMtrl"));

	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, TEX_LOAD(L"texture\\Battle\\ingame_blackbox1.png"));



	CGameObject* pObj;

	//===========
	// Field 
	//===========
	pObj = new CGameObject;
	pObj->SetName(L"FieldObject");
	m_Field = new CFieldScript;
	pObj->AddComponent(m_Field);
	OBJ->AddChild(pObj);
	m_Field->LoadDeckInfoFromFile();
	m_Field->SetFieldPos(Vec3(0.f, -290.f, 1000.f));
	m_Field->SetModeMgr(this);

	//===========
	// SummonBtn 
	//===========
	pObj = new CGameObject;
	pObj->SetName(L"SummonBtn");
	m_SummonBtn = new CSummonDiceBtnScript;
	pObj->AddComponent(m_SummonBtn);
	OBJ->AddChild(pObj);
	m_SummonBtn->SetDiceBtnPos(Vec3(0.f, -390.f, 900.f));
	m_SummonBtn->SetModeMgr(this);

	//===========
	// SP Bar
	//===========
	pObj = new CGameObject;
	pObj->SetName(L"SPBar");
	m_SPBar = new CSPBarScript;
	pObj->AddComponent(m_SPBar);
	OBJ->AddChild(pObj);
	m_SPBar->SetModeMgr(this);
	m_SPBar->SetSPBarPos(Vec3(-135.f, -400.f, 900.f));

	//===========
	// Player HP
	//===========
	pObj = new CGameObject;
	pObj->SetName(L"Player_HP");
	m_HP = new CPlayerHP;
	pObj->AddComponent(m_HP);
	OBJ->AddChild(pObj);
	m_HP->SetModeMgr(this);
	m_HP->SetPlayerHPPos(Vec3(-125.f, -365.f, 900.f));

	//===========
	// Wave Timer
	//===========
	pObj = new CGameObject;
	pObj->SetName(L"Wave_Timer");
	m_WaveTimer = new CWaveTimer;
	m_WaveTimer->SetModeMgr(this);
	pObj->AddComponent(m_WaveTimer);
	OBJ->AddChild(pObj);

	//==================
	// FONTINFO ���� 
	//==================
	m_WaveFontInfo.Color = FONT_RGBA(255, 255, 255, 255);
	m_WaveFontInfo.fFontSize = 20.f;
	m_WaveFontInfo.FontType = FONT_TYPE::MAPLE;
	m_WaveFontInfo.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_WaveFontInfo.IsWorldPosRender = true;
	m_WaveFontInfo.fPosX = -220;
	m_WaveFontInfo.fPosY = 90;
	m_WaveFontInfo.WStr = wstring(L"WAVE 1");

	// �⺻ ���� ����
	m_CurWave = 1;
	m_CurWaveTime = m_MaxWaveTime;
	
	// �⺻ ���� ����
	// ���߿� �������� ������ �� �����غ���
	m_CurBoss = BOSS_TYPE::RANDOM_KNIGHT;

	m_Field->SetCurWave(m_CurWave);
}

void CPracticeModeMgr::tick()
{
	CFontMgr::GetInst()->AddRenderFont(m_WaveFontInfo);

	// ���̺� ����
	if (m_IsBossAppears == false)
	{
		m_CurWaveTime -= DT;

		if (m_CurWaveTime < 0.f)
		{
			// ���̺� �ð��� ������ ��� ���� ����
			BossAppears();
		}
	}
	// ������ óġ�� ��� ���̺� ����(���� ���̺� ����)
	else if (!m_Field->IsBossAlive())
	{
		WaveEnd();
	}
}

#undef OBJ
