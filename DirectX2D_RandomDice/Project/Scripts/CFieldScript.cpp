#include "pch.h"
#include "CFieldScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>

#include "CDiceScript.h"
#include "CDiceUI.h"
#include "CEnemyGateScript.h"




CFieldScript::CFieldScript()
	:CScript(FIELDSCRIPT)
	, AutoSpawnEnemy(true)
	, m_SummonSP(10)
	, m_SP(100)
	, m_MaxEnemyHP(0)
	, m_gen(m_rd())
	, m_XDis(0, 4)
	, m_YDis(0, 2)
	, m_CurWave(1)
{

	// ���߿��� ���⼭ �ʱ�ȭ�ϸ� �ȵȴ�.

	m_Deck[0] = DICE(1);
	m_Deck[1] = DICE(2);
	m_Deck[2] = DICE(3);
	m_Deck[3] = DICE(4);
	m_Deck[4] = DICE(5);
}

CFieldScript::~CFieldScript()
{
}

#define OBJECT			GetOwner()

void CFieldScript::begin()
{
	CDiceScript::InitDicePath();


	// ���� ó��
	
	assert(OBJECT);

	if (OBJECT->GetRenderComponent() == nullptr)
	{
		OBJECT->AddComponent(new CMeshRender);
		OBJECT->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		OBJECT->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
	}

	assert(OBJECT->MeshRender());
	OBJECT->MeshRender()->GetDynamicMaterial();

	CGameObject* pObj = nullptr;
	wstring wstrPath;

	wstrPath = L"texture\\BattleField\\battlefield_normal_bg_top.png";
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));


	// �÷��̾� �ڽ��� ��� -> Vec3(0, -210, 1000)
	// �� �÷��̾��� ��� -> Vec3(0, 340, 1000)
	//OBJECT->Transform()->SetRelativePos(Vec3(0, -210, 1000));
	OBJECT->Transform()->SetRelativeScale(Vec3(540, 540, 1));

	Vec3 FieldPos = OBJECT->Transform()->GetRelativePos();


	// MeshRender, Transform, alphablend shader, mtrl �޸� ������. ��� ���� ������Ʈ�� ��� ����
	wstrPath = L"prefab\\AlphaBlendGameObject.pref";
	Ptr<CPrefab> pAlphaPref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	// MeshRender, Transform, Std2D shader, mtrl �޸� ������. ��� ���� ������Ʈ�� ��� ����
	wstrPath = L"prefab\\Std2DGameObject.pref";
	Ptr<CPrefab> pStd2DPref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	
	// �ֻ��� ������
	wstrPath = L"prefab\\Dice.pref";
	Ptr<CPrefab> pDicePref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
#pragma region _		Set Dice

			// ������ ��üȭ
			CGameObject* pDice = pDicePref->Instantiate();
			OBJECT->AddChild(pDice);

			// ��� ��ġ�� ���� Pos ����(z : 500)
			Vec3 dicePos = Vec3(((int)i - 2) * 62, ((int)j - 2) * 62 + 120, -500);
			pDice->Transform()->SetRelativePos(dicePos);

			// ��ũ��Ʈ �����ͼ� ����
			CDiceScript* DiceScript = m_DiceField[i][j] = pDice->GetScript<CDiceScript>();

			// �ֻ��� ��ũ��Ʈ�� �ֹ����� ����
			DiceScript->SetField(this);

			// �ֻ��� ��ũ��Ʈ�� ���, Pos, Name ����
			DiceScript->SetDiceXY(i + 1, j + 1);

			// �ֻ��� ������ ���� ���� 
			DiceScript->SetDiceType(DICE::NONE);

			// AddObject

#pragma endregion

#pragma region _		Background Object

			//////////////////////////////////
			// battlefield_normal_dicebg_1 ~ 3
			//////////////////////////////////
	
			pObj = pAlphaPref->Instantiate();
			
			// �ֻ��� pos �� �״�� ���
			pObj->Transform()->SetRelativePos(Vec3(dicePos.x, dicePos.y, -20));
			pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
			pObj->Transform()->SetAbsolute(true);

			wstrPath = L"texture\\BattleField\\Dice\\battlefield_normal_dicebg_" + to_wstring((i + j) % 3 + 1) + L".png";
			pObj->SetName(wstrPath);

			pObj->MeshRender()->GetDynamicMaterial();
			pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));


			OBJECT->AddChild(pObj);

		}
	}

	////////////////////////////////
	// battlefield_normal_bottom_bg
	////////////////////////////////
	
	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deck\\battlefield_normal_bottom_bg.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(0.f, -215.f, -30));
	pObj->Transform()->SetRelativeScale(Vec3(510.f, 110.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);


	////////////////////////////////
	// battlefield_normal_vs_line_1
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_1.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-230.f, 50.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(33.f, 340.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_vs_line_2
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_2.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-10.f, 213.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(407.f, 19.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));
	
	m_Line2 = pObj;

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_vs_line_3
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_3.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(216.f, 50.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(46.f, 343.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_deco_1
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_1.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(180.f, -70.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(75.f, 75.f, 1.f));
	pObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, XM_PI / 3));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_deco_2
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_2.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(189.f, 109.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 40.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_deco_4
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_4.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-190.f, 165.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////
	// ingame_mob_gate 1
	////////////////////

	pObj = new CGameObject;
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(-235.f, -110.f, -20));
	pObj->Transform()->SetAbsolute(true);

	pObj->AddComponent(new CEnemyGateScript);
	pObj->SetName(L"ingame_mob_gate_1");
	
	m_EnemyGate1 = pObj;

	OBJECT->AddChild(pObj);

	////////////////////
	// ingame_mob_gate 2
	////////////////////

	pObj = new CGameObject;
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(230.f, -110.f, -20));
	pObj->Transform()->SetAbsolute(true);

	pObj->AddComponent(new CEnemyGateScript);
	pObj->SetName(L"ingame_mob_gate_2");
	
	m_EnemyGate2 = pObj;

	OBJECT->AddChild(pObj);

#pragma endregion


	//==================
	// EnemyPrefab ����
	//==================
	
	wstrPath = L"prefab\\DefaultEnemy.pref";
	m_EnemyPrefab[(UINT)ENEMY_TYPE::DEFAULT] = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);
	
	wstrPath = L"prefab\\SpeedEnemy.pref";
	m_EnemyPrefab[(UINT)ENEMY_TYPE::SPEED] = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);
	
	wstrPath = L"prefab\\BigEnemy.pref";
	m_EnemyPrefab[(UINT)ENEMY_TYPE::BIG] = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	assert(m_EnemyPrefab[(UINT)ENEMY_TYPE::DEFAULT].Get());
	assert(m_EnemyPrefab[(UINT)ENEMY_TYPE::SPEED].Get());
	assert(m_EnemyPrefab[(UINT)ENEMY_TYPE::BIG].Get());



	//==================
	// FONTINFO ���� 
	//==================
	m_fInfo1.Color = FONT_RGBA(0, 0, 0, 255);
	m_fInfo1.fFontSize = 15.f;
	m_fInfo1.FontType = FONT_TYPE::ALBA_SUPER;
	m_fInfo1.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;

	m_fInfo2.Color = FONT_RGBA(255, 255, 255, 255);
	m_fInfo2.fFontSize = 15.2f;
	m_fInfo2.FontType = FONT_TYPE::ALBA_MATTER;
	m_fInfo2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;


	// �ʱ� ���� Ÿ�̹� ����
	m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::DEFAULT] = 4.f;
	m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::SPEED] = 7.f;
	m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::BIG] = 13.f;



	//===================
	// CDiceUI �� ���� ����
	//===================

	for (UINT i = 0; i < 5; ++i)
	{
		// �� �ڽ� ������Ʈ ����
		pObj = new CGameObject;
		OBJECT->AddChild(pObj);

		// DiceUI �߰�
		CDiceUI* pDiceUI = new CDiceUI;
		pObj->AddComponent(pDiceUI);
		pObj->SetName(L"DiceUI_" + to_wstring(i));

		// begin���� �ʿ��� ���� ����
		pDiceUI->SetDice(m_Deck[i]);
		pDiceUI->SetDeckIdx(i);
		pDiceUI->SetField(this);
	}







	//==================
	// Debug
	//==================
	AddScriptParam(SCRIPT_PARAM::INT, "Cur Wave", &m_CurWave);
	AddScriptParam(SCRIPT_PARAM::INT, "Cur SP", &m_SP);
	AddScriptParam(SCRIPT_PARAM::INT, "Enemy Auto Spawn", &AutoSpawnEnemy);
	AddScriptParam(SCRIPT_PARAM::INT, "DEFAULT Enemy Spawn Count", &m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount);
	AddScriptParam(SCRIPT_PARAM::INT, "BIG Enemy Spawn Count", &m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount);
	AddScriptParam(SCRIPT_PARAM::INT, "SPEED Enemy Spawn Count", &m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount);


}

void CFieldScript::tick()
{

	// AccSpawnCoolDown
	for (UINT i = 0; i < (UINT)ENEMY_TYPE::END; ++i)
	{
		m_AccSpawnCoolDown[i] -= m_EnemySpawnRate[m_CurWave] * DT;

		// m_SpawnEnemyCheck�� ��ٿ� ����
		m_SpawnEnemyCheck[i].CoolDown -= DT;

		// ��Ÿ���� �������� 0���� �ʱ�ȭ �س���
		if (m_SpawnEnemyCheck[i].CoolDown < 0.f)
			m_SpawnEnemyCheck[i].CoolDown = 0.f;
	}

	// AutoSpawnEnemy
	if (AutoSpawnEnemy)
	{
		if (m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::DEFAULT] < 0.f)
		{
			m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::DEFAULT] = 2.f;
			++m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount;
		}
		if (m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::SPEED] < 0.f)
		{
			m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::SPEED] = 7.f;
			++m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount;
		}
		if (m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::BIG] < 0.f)
		{
			m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::BIG] = 10.f;
			++m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount;
		}
	}


	//======================
	// Update MaxEnemyHealth
	//======================

	m_EnemyHPUpdateTimer += DT;

	if (m_EnemyHPUpdateTimer > 5.f)
	{
		m_MaxEnemyHP += m_EnemyHPArr[m_CurWave];
		m_EnemyHPUpdateTimer = 0.f;
	}

	/////

	//==================
	// Spawn Enemy Check
	//==================

	Vec3 Line1StartPos(m_EnemyGate1->Transform()->GetWorldPos().x, m_EnemyGate1->Transform()->GetWorldPos().y, 600);
	Vec3 Line2StartPos(m_EnemyGate1->Transform()->GetWorldPos().x, m_Line2->Transform()->GetWorldPos().y, 600);
	Vec3 Line3StartPos(m_EnemyGate2->Transform()->GetWorldPos().x, m_Line2->Transform()->GetWorldPos().y, 600);
	Vec3 LineEndPos	  (m_EnemyGate2->Transform()->GetWorldPos().x, m_EnemyGate2->Transform()->GetWorldPos().y, 600);

	float YLineLen = Line2StartPos.y - Line1StartPos.y;
	float XLineLen = Line3StartPos.x - Line2StartPos.x;

	ENEMY_PAIR SpawnEnemy{};

	///// DEFAULT
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnableSpawn())
	{

		// 1. ������ ��üȭ
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::DEFAULT]->Instantiate();
		SpawnEnemy.pObject->SetName(L"DefaultEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// ���� ����Ʈ Pos
		SpawnEnemy.pEnemyScript->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Line1StartPos);

		// ü�� ����
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP);
		SpawnEnemy.pEnemyScript->SetField(this);

		// 2. EnemyList�� ����
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject�� ���ӿ� ����
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount;
	}

	///// SPEED
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnableSpawn())
	{

		// 1. ������ ��üȭ
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::SPEED]->Instantiate();
		SpawnEnemy.pObject->SetName(L"SpeedEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// ���� ����Ʈ Pos
		SpawnEnemy.pObject->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Line1StartPos);

		// ü�� ����
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP);
		SpawnEnemy.pEnemyScript->SetField(this);

		// 2. EnemyList�� ����
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject�� ���ӿ� ����
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount;
	}

	///// BIG
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnableSpawn())
	{

		// 1. ������ ��üȭ
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::BIG]->Instantiate();
		SpawnEnemy.pObject->SetName(L"BigEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// ���� ����Ʈ Pos
		SpawnEnemy.pObject->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Vec3(Line1StartPos.x, Line1StartPos.y, 599.f));
		
		// ü�� ����
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP * 5);
		SpawnEnemy.pEnemyScript->SetField(this);
		
		// 2. EnemyList�� ����
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject�� ���ӿ� ����
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);


		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount;
	}

	
	///////////////////

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	for (auto it = m_EnemyList.begin(); it != m_EnemyList.end(); )
	{
		ENEMY_PAIR pEnemy = *it;

		CGameObject* pObject = pEnemy.pObject;
		CEnemyScript* pEScript = pEnemy.pEnemyScript;

		bool ThisEnemyIsDead = false;

		//==================
		// Set Enemy Pos
		//==================

		// ��ġ ���� - 31 : 38 : 31
		float MoveProgress = pEScript->GetMoveProgress();

		// ATTACK_PRIORITY Check - FRONT
		if (m_AttackPriority[(UINT)ATTACK_PRIORITY::FRONT].pObject == nullptr 
			|| (m_AttackPriority[(UINT)ATTACK_PRIORITY::FRONT].pEnemyScript->GetMoveProgress() < MoveProgress
			&& MoveProgress < 100.f))
		{
			// ���� �켱 ���� ������Ʈ�� nullptr�� ���
			// ���൵�� ���� �ռ��� ���
			m_AttackPriority[(UINT)ATTACK_PRIORITY::FRONT] = pEnemy;
		}

		Vec3 Pos(0.f, 0.f, 600.f);



		if (MoveProgress <= 31.f) // �� �� ���ο� ��ġ
		{
			Pos = Line1StartPos;
			Pos.y += YLineLen * (MoveProgress / 31.f);
		}
		else if (MoveProgress > 69.f) // ���� �� ���ο� ��ġ
		{
			if (MoveProgress > 100.f)
			{
				Pos = LineEndPos;
				Pos.z = OBJECT->Transform()->GetRelativePos().z + 100;
				
				ThisEnemyIsDead = true;
			}
			else
			{
				Pos = Line3StartPos;
				Pos.y -= YLineLen * ((MoveProgress - 69.f) / 31.f);
			}
		}
		else // �߾� ���ο� ��ġ
		{
			Pos = Line2StartPos;
			Pos.x += XLineLen * ((MoveProgress - 31.f) / 38.f);
		}

		if (pEScript->GetEnemyType() == ENEMY_TYPE::BIG)
			Pos.z += 2;

		if (pEScript->GetEnemyType() == ENEMY_TYPE::SPEED)
			Pos.z -= 2;

		pObject->Transform()->SetRelativePos(Pos);


		//==================
		// Check Enemy Dead
		//==================

		if (!pEScript->IsEndDeathParticle())
		{
			if (ThisEnemyIsDead)
			{
				pEScript->PlayDeathParticle();
			}
			else
			{

				//=========
				// HP Text
				//=========
				
				// ü�� ��Ʈ�� ��ġ ����
				m_fInfo1.fPosX = Pos.x + (vResol.x / 2);
				m_fInfo1.fPosY = -Pos.y + (vResol.y / 2) + 7;
				m_fInfo2.fPosX = Pos.x + (vResol.x / 2);
				m_fInfo2.fPosY = -Pos.y + (vResol.y / 2) + 10;

				// ü�� ��Ʈ�� ���ڿ� ����
				UINT enemyHP = pEScript->GetEnemyHealth();

				// ���� ���߱�
				if (enemyHP < 10000)
				{
					m_fInfo1.WStr = to_wstring(enemyHP);
					m_fInfo2.WStr = to_wstring(enemyHP);
				}
				else if (enemyHP < 1000000) // enemyHP �� 10000���� Ŀ�� ��� �������� ���� 1000�� ������ K�� ǥ��
				{
					enemyHP /= 1000;
					m_fInfo1.WStr = to_wstring(enemyHP) + L"K";
					m_fInfo2.WStr = to_wstring(enemyHP) + L"K";
				}
				else // enemyHP �� 1000000���� Ŀ�� ��� �������� ���� 1000000�� ������ M�� ǥ��
				{
					enemyHP /= 1000000;
					m_fInfo1.WStr = to_wstring(enemyHP) + L"M";
					m_fInfo2.WStr = to_wstring(enemyHP) + L"M";
				}
				
				// ��Ʈ �Ŵ��� ��¿� �߰�
				CFontMgr::GetInst()->AddRenderFont(m_fInfo2);
				CFontMgr::GetInst()->AddRenderFont(m_fInfo1);
			}


			++it;
		}
		// ���� ��� ��ƼŬ ����� ������ ���
		else
		{
			// �������� ����
			GamePlayStatic::DestroyGameObject(pObject);

			// EnemyList���� ����
			it = m_EnemyList.erase(it);
		}

	}


}


UINT CFieldScript::DiceLevelUp(DICE _dice, UINT _idx)
{
	if (m_DiceLevel[_idx] == 5)
		return 0;

	// ��� �ֻ����� ��ȸ�ϸ鼭 _dice�� ���� ������ �ֻ����� �� ��������
	
	// �ش� �ֻ����鿡�� ������ �Լ� ȣ��



	// �ֻ��� ������
	// �ö� ��� �ε��� ��ȯ
	return m_LevelUpSP[++m_DiceLevel[_idx] - 1];
}

UINT CFieldScript::GetCurDiceLevel(DICE _dice) const
{
	int i = 0;
	for (; i <= 5; ++i)
	{
		if (i == 5)
			return 0;
		if (m_Deck[i] == _dice)
			return m_DiceLevel[i];
	}
}

void CFieldScript::SummonDice()
{ 
	// �ʵ尡 ��� �� �ְų�, ���� SP�� ��ȯ SP���� ���� ��� return
	if (m_CurDiceCount >= 15 || m_SP < m_SummonSP)
		return;

	// SP �Ҹ��ϱ�
	m_SP -= m_SummonSP;

	// ��ȯ SP ����
	m_SummonSP += 10;


	CDiceScript* pDice = nullptr;

	// ������ �� �ʵ� ���� ã��
	while (true)
	{
		pDice = GetRandomDice();
		if (pDice != nullptr && pDice->GetDice() == DICE::NONE)
			break;
	}

	// ���߿��� �� �߿� ��� �־��־�� �Ѵ�.
	pDice->SetDiceType(m_Deck[m_XDis(m_gen)]);

	++m_CurDiceCount;
}

void CFieldScript::SpawnDice(UINT _LOW, UINT _COL)
{
}

#undef OBJECT

