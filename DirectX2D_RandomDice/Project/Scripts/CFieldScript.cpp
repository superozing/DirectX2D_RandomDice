#include "pch.h"
#include "CFieldScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>

#include "CDiceScript.h"
#include "CDiceUI.h"
#include "CEnemyScript.h"
#include "CEnemyGateScript.h"
#include "CSnakeBoss.h"
#include "CSilenceBoss.h"
#include "CRandomKnightBoss.h"

#include "CPracticeModeMgr.h"


// Functor
struct CompareEnemyPair
{
	bool operator()(const ENEMY_PAIR& enemy1, const ENEMY_PAIR& enemy2) const
	{
		// �������� ���� ��� ������ �����Ͽ� ��
		return enemy1.pEnemyScript->GetMoveProgress() < enemy2.pEnemyScript->GetMoveProgress();
	}
};

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
	, IsInvincible(false)
{

}

CFieldScript::~CFieldScript()
{
}

#define OBJECT			GetOwner()


void CFieldScript::tick()
{

	//==========================
	// AttackPriority �迭 Clear
	//==========================
	for (UINT i = 0; i < (UINT)ATTACK_PRIORITY::END; ++i)
		m_AttackPriority[i] = ENEMY_PAIR();

	for (UINT i = 0; i < (UINT)ENEMY_TYPE::END; ++i)
	{
		// Enemy Auto Spawn Timer
		// ���� ���̺��� ������ Enemy Auto Spawn Timer�� ����� �Ѵ�.
		if (m_CurWave != BOSS_WAVE)
			m_AccSpawnCoolDown[i] -= m_EnemySpawnRate[m_CurWave] * DT;

		// Enemy Spawn ����
		//	- ���� ���� ��ȯ�Ǵ°� ������.
		m_SpawnEnemyCheck[i].CoolDown -= DT;

		//  Enemy Spawn ������ �������� 0���� �ʱ�ȭ �س���
		if (m_SpawnEnemyCheck[i].CoolDown < 0.f)
			m_SpawnEnemyCheck[i].CoolDown = 0.f;
	}

	//======================
	// Enemy Auto Spawn
	//======================
	//	- ���� ������ �������Ѽ� ���Ŀ� ���ο� �� ������Ʈ�� �߰��ϵ��� ��.
	if (AutoSpawnEnemy) // ����׿� bool ��.
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
		// 5�ʸ��� ���� ü���� ���� ���忡 ����ؼ� ������Ŵ.
		m_MaxEnemyHP += m_EnemyHPArr[m_CurWave];
		m_EnemyHPUpdateTimer = 0.f;
	}


	//===============
	// Spawn Enemy
	//===============
	Vec3 Line1StartPos(m_EnemyGate1->Transform()->GetWorldPos().x, m_EnemyGate1->Transform()->GetWorldPos().y, 600);
	Vec3 Line2StartPos(m_EnemyGate1->Transform()->GetWorldPos().x, m_Line2->Transform()->GetWorldPos().y, 600);
	Vec3 Line3StartPos(m_EnemyGate2->Transform()->GetWorldPos().x, m_Line2->Transform()->GetWorldPos().y, 600);
	Vec3 LineEndPos(m_EnemyGate2->Transform()->GetWorldPos().x, m_EnemyGate2->Transform()->GetWorldPos().y, 600);

	float YLineLen = Line2StartPos.y - Line1StartPos.y;
	float XLineLen = Line3StartPos.x - Line2StartPos.x;

	ENEMY_PAIR SpawnEnemy{};

	// ���� ���̺갡 �Ϲ� ���̺�
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

	// ���� ���̺갡 ���� ���̺�
	if (m_CurWave == BOSS_WAVE)
	{
		///// BOSS
		if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BOSS].EnemySpawnCount != 0
			&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BOSS].EnableSpawn())
		{
			BOSS_TYPE CurBoss = (BOSS_TYPE)m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BOSS].EnemySpawnCount;
			// 1. ������ ��üȭ
			SpawnEnemy.pObject = m_BossPrefab[(UINT)CurBoss]->Instantiate();

			SpawnEnemy.pObject->SetName(L"BossEnemy");

			switch (CurBoss)
			{
			case BOSS_TYPE::SNAKE : 
				SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CSnakeBoss>();
				break;
			case BOSS_TYPE::SILENCE :
				SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CSilenceBoss>();
				break;
			case BOSS_TYPE::RANDOM_KNIGHT :
				SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CRandomKnightBoss>();
				break;
			}

			// ���� ����Ʈ Pos
			SpawnEnemy.pObject->begin();
			SpawnEnemy.pObject->Transform()->SetRelativePos(Vec3(Line1StartPos.x, Line1StartPos.y, 599.f));

			// ü�� ����
			SpawnEnemy.pEnemyScript->SetEnemyHealth(m_AccEnemyHP);
			SpawnEnemy.pEnemyScript->SetField(this);

			// 2. EnemyList�� ����
			m_EnemyList.push_back(SpawnEnemy);

			// 3. SpawnObject�� ���ӿ� ����
			GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

			m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BOSS].EnemySpawnCount = 0;
		}
	}

	///////////////////
	///////////////////
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

		Vec3 Pos(0.f, 0.f, 600.f);

		// ���ο� Enemy Pos Set
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

		if (pEScript->GetEnemyType() == ENEMY_TYPE::SPEED)
			Pos.z -= 2;

		pObject->Transform()->SetRelativePos(Pos);


		//==================
		// Check Enemy Dead
		//==================

		// DamageCheck
		if (pEScript->GetEnemyHealth() <= 0)
			ThisEnemyIsDead = true;

		if (ThisEnemyIsDead)
		{
			Pos.z = Transform()->GetWorldPos().z + 100;
			pObject->Transform()->SetRelativePos(Pos);
			pEScript->SetDeadEnemy();

			// EnemyList���� ����
			it = m_EnemyList.erase(it);
		}
		else
		{
			++it;
		}
	}

	m_EnemyList.sort(CompareEnemyPair());

	//====================
	// Attack Priority Set
	//====================

	if (!m_EnemyList.empty())
	{
		// ATTACK_PRIORITY::FRONT
		m_AttackPriority[(UINT)ATTACK_PRIORITY::FRONT] = m_EnemyList.back();

		// ATTACK_PRIORITY::HIGH_HEALTH
		m_AttackPriority[(UINT)ATTACK_PRIORITY::HIGH_HEALTH] = m_EnemyList.front();
		for (auto& it : m_EnemyList)
		{
			if (it.pEnemyScript->GetEnemyHealth() > m_AttackPriority[(UINT)ATTACK_PRIORITY::HIGH_HEALTH].pEnemyScript->GetEnemyHealth())
				m_AttackPriority[(UINT)ATTACK_PRIORITY::HIGH_HEALTH] = it;
		}
	}
	else if (m_EnemyList.empty() && m_CurWave == BOSS_WAVE)
	{
		// ���� ���̺� ���� ����
		m_ModeMgr->WaveEnd();
		
		ClearEnemyList();

		for (UINT i = 0; i < (UINT)ENEMY_TYPE::BOSS; ++i)
		{
			m_SpawnEnemyCheck[i].EnemySpawnCount = 0;
			m_SpawnEnemyCheck[i].CoolDown = 0;
		}
	}

}


UINT CFieldScript::DiceLevelUp(DICE _dice, UINT _idx)
{
	if (m_DiceLevel[_idx] == 5)
		return 0;

	if (m_LevelUpSP[m_DiceLevel[_idx] - 1] > m_SP)
		return m_LevelUpSP[m_DiceLevel[_idx] - 1];

	// ��� �ֻ����� ��ȸ�ϸ鼭 _dice�� ���� ������ �ֻ����� �� ��������
	for (UINT i = 0; i < 5; ++i)
	{
		for (UINT j = 0; j < 3; ++j)
		{
			CDiceScript* pDice = m_DiceField[i][j];

			if (pDice->GetDice() == _dice)
			{
				// �ش� �ֻ����鿡�� ������ �Լ� ȣ��
				pDice->PlayLevelUp();
			}
		}
	}

	// �ֻ��� ������
	m_SP -= m_LevelUpSP[m_DiceLevel[_idx] - 1];

	// �ö� ��� �ε��� ��ȯ
	return m_LevelUpSP[++m_DiceLevel[_idx] - 1];
}

void CFieldScript::SetDiceMergeState()
{
	// ���� ���� ���� ����
	for (UINT i = 0; i < 5; ++i)
	{
		for (UINT j = 0; j < 3; ++j)
		{
			if (m_DiceField[i][j]->IsMergeAbleDice())
			{
				m_DiceField[i][j]->SetMergeState(DICE_MERGE_STATE::ABLE);
			}
			else
			{
				m_DiceField[i][j]->SetMergeState(DICE_MERGE_STATE::UNABLE);
			}
		}
	}
}

void CFieldScript::SetCurWave(UINT _Wave)
{
	m_AccEnemyHP = ClearEnemyList();

	// ���� ���� ���̺갡 ���� ���̺��� ���, ��� �Ŵ����κ��� ���� ������ �������� ��ȯ.
	if (_Wave == BOSS_WAVE)
	{
		m_AccEnemyHP = 10000 * m_CurWave + (m_AccEnemyHP / 2);
		SpawnBoss(m_ModeMgr->GetCurBossType());

		for (UINT i = 0; i < (UINT)ENEMY_TYPE::BOSS; ++i)
		{
			m_SpawnEnemyCheck[i].EnemySpawnCount = 0;
			m_SpawnEnemyCheck[i].CoolDown = 0;
		}
	}

	m_CurWave = _Wave;
}


void CFieldScript::SpawnBoss(BOSS_TYPE _BossType)
{
	m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BOSS].EnemySpawnCount = (UINT)_BossType;
}

int CFieldScript::ClearEnemyList()
{
	int AccEnemyHealth = 0;

	auto it = m_EnemyList.begin();

	float FieldZPos = 0.f;
	
	if (Transform())
		FieldZPos = Transform()->GetRelativePos().z;
	else
		return 0;

	for (;it != m_EnemyList.end();)
	{
		Vec3 pos = it->pObject->Transform()->GetRelativePos();
		pos.z = FieldZPos + 150;
		it->pObject->Transform()->SetRelativePos(pos);

		AccEnemyHealth += max(it->pEnemyScript->GetEnemyHealth(), 0);

		it->pEnemyScript->SetEnemyHealth(0);
		it->pEnemyScript->SetDeadEnemy();
		it = m_EnemyList.erase(it);
	}

	return AccEnemyHealth;
}


void CFieldScript::LoadDeckInfoFromFile()
{
	wstring Path = CPathMgr::GetContentPath();

	Path += L"userdata\\player_deck.txt";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, Path.c_str(), L"r");

	if (nullptr == pFile)
	{
		MessageBox(nullptr, L"���� ����", L"Deck Info Load ����", MB_OK);
		return;
	}


	while (true)
	{
		wchar_t szRead[256] = {};
		if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
			break;

		if (!wcscmp(szRead, L"[DICE_1]"))
		{
			fwscanf_s(pFile, L"%s", szRead, 256);
			m_Deck[0] = (DICE)stoi(szRead);
		}
		else if (!wcscmp(szRead, L"[DICE_2]"))
		{
			fwscanf_s(pFile, L"%s", szRead, 256);
			m_Deck[1] = (DICE)stoi(szRead);
		}
		else if (!wcscmp(szRead, L"[DICE_3]"))
		{
			fwscanf_s(pFile, L"%s", szRead, 256);
			m_Deck[2] = (DICE)stoi(szRead);
		}
		else if (!wcscmp(szRead, L"[DICE_4]"))
		{
			fwscanf_s(pFile, L"%s", szRead, 256);
			m_Deck[3] = (DICE)stoi(szRead);
		}
		else if (!wcscmp(szRead, L"[DICE_5]"))
		{
			fwscanf_s(pFile, L"%s", szRead, 256);
			m_Deck[4] = (DICE)stoi(szRead);
		}
	}
	fclose(pFile);
}

void CFieldScript::SaveDeckInfoFromFile()
{
	wstring Path = CPathMgr::GetContentPath();

	Path += L"userdata\\player_deck.txt";
	
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, Path.c_str(), L"w");

	if (nullptr == pFile)
	{
		MessageBox(nullptr, L"���� ����", L"Deck Info Save ����", MB_OK);
		return;
	}

	for (UINT i = 0; i < 5; ++i)
	{
		wstring strDice = L"[DICE_";
		strDice += to_wstring(i + 1) + L"]";

		fwprintf_s(pFile, strDice.c_str());
		fwprintf_s(pFile, L"\n");
		fwprintf_s(pFile, to_wstring((UINT)m_Deck[i]).c_str());
		fwprintf_s(pFile, L"\n\n");

	}
	fclose(pFile);
}

void CFieldScript::SetFocusDice(CDiceScript* _Dice)
{
	m_FocusDice = _Dice;

	if (m_FocusDice)
	{
		SetDiceMergeState();
		auto pos = m_FocusDice->Transform()->GetRelativePos();
		m_FocusMarker->Transform()->SetRelativePos(Vec3(pos.x, pos.y, pos.z - 10));
	}
	else // nullptr�� ���õ� ���
	{
		m_FocusMarker->Transform()->SetRelativePos(Vec3(0, 0, 10));

		// ���� ���� ����
		for (UINT i = 0; i < 5; ++i)
		{
			for (UINT j = 0; j < 3; ++j)
			{
				m_DiceField[i][j]->SetMergeState(DICE_MERGE_STATE::NOT_SET);
			}
		}
	}
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

	return 0;
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
		pDice = GetRandomFieldDice();
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

