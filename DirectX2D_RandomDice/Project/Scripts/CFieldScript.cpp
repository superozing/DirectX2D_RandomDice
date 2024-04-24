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
		// 기준으로 삼을 멤버 변수에 접근하여 비교
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
	// AttackPriority 배열 Clear
	//==========================
	for (UINT i = 0; i < (UINT)ATTACK_PRIORITY::END; ++i)
		m_AttackPriority[i] = ENEMY_PAIR();

	for (UINT i = 0; i < (UINT)ENEMY_TYPE::END; ++i)
	{
		// Enemy Auto Spawn Timer
		// 보스 웨이브일 때에는 Enemy Auto Spawn Timer가 멈춰야 한다.
		if (m_CurWave != BOSS_WAVE)
			m_AccSpawnCoolDown[i] -= m_EnemySpawnRate[m_CurWave] * DT;

		// Enemy Spawn 간격
		//	- 적이 겹쳐 소환되는걸 방지함.
		m_SpawnEnemyCheck[i].CoolDown -= DT;

		//  Enemy Spawn 간격이 돌았으면 0으로 초기화 해놓기
		if (m_SpawnEnemyCheck[i].CoolDown < 0.f)
			m_SpawnEnemyCheck[i].CoolDown = 0.f;
	}

	//======================
	// Enemy Auto Spawn
	//======================
	//	- 스폰 개수를 증가시켜서 이후에 라인에 적 오브젝트를 추가하도록 함.
	if (AutoSpawnEnemy) // 디버그용 bool 값.
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
		// 5초마다 적의 체력을 현재 라운드에 비례해서 증가시킴.
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

	// 현재 웨이브가 일반 웨이브
	///// DEFAULT
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnableSpawn())
	{

		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::DEFAULT]->Instantiate();
		SpawnEnemy.pObject->SetName(L"DefaultEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pEnemyScript->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Line1StartPos);

		// 체력 설정
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP);
		SpawnEnemy.pEnemyScript->SetField(this);

		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount;
	}

	///// SPEED
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnableSpawn())
	{

		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::SPEED]->Instantiate();
		SpawnEnemy.pObject->SetName(L"SpeedEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pObject->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Line1StartPos);

		// 체력 설정
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP);
		SpawnEnemy.pEnemyScript->SetField(this);

		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount;
	}

	///// BIG
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnableSpawn())
	{

		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::BIG]->Instantiate();
		SpawnEnemy.pObject->SetName(L"BigEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pObject->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Vec3(Line1StartPos.x, Line1StartPos.y, 599.f));

		// 체력 설정
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP * 5);
		SpawnEnemy.pEnemyScript->SetField(this);

		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);


		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount;
	}

	// 현재 웨이브가 보스 웨이브
	if (m_CurWave == BOSS_WAVE)
	{
		///// BOSS
		if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BOSS].EnemySpawnCount != 0
			&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BOSS].EnableSpawn())
		{
			BOSS_TYPE CurBoss = (BOSS_TYPE)m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BOSS].EnemySpawnCount;
			// 1. 프리팹 객체화
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

			// 시작 게이트 Pos
			SpawnEnemy.pObject->begin();
			SpawnEnemy.pObject->Transform()->SetRelativePos(Vec3(Line1StartPos.x, Line1StartPos.y, 599.f));

			// 체력 설정
			SpawnEnemy.pEnemyScript->SetEnemyHealth(m_AccEnemyHP);
			SpawnEnemy.pEnemyScript->SetField(this);

			// 2. EnemyList에 삽입
			m_EnemyList.push_back(SpawnEnemy);

			// 3. SpawnObject로 게임에 생성
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

		// 위치 비율 - 31 : 38 : 31
		float MoveProgress = pEScript->GetMoveProgress();

		Vec3 Pos(0.f, 0.f, 600.f);

		// 라인에 Enemy Pos Set
		if (MoveProgress <= 31.f) // 왼 쪽 라인에 위치
		{
			Pos = Line1StartPos;
			Pos.y += YLineLen * (MoveProgress / 31.f);
		}
		else if (MoveProgress > 69.f) // 오른 쪽 라인에 위치
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
		else // 중앙 라인에 위치
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

			// EnemyList에서 제거
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
		// 보스 웨이브 종료 시점
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

	// 모든 주사위를 순회하면서 _dice와 같은 유형의 주사위를 싹 가져오기
	for (UINT i = 0; i < 5; ++i)
	{
		for (UINT j = 0; j < 3; ++j)
		{
			CDiceScript* pDice = m_DiceField[i][j];

			if (pDice->GetDice() == _dice)
			{
				// 해당 주사위들에게 레벨업 함수 호출
				pDice->PlayLevelUp();
			}
		}
	}

	// 주사위 레벨업
	m_SP -= m_LevelUpSP[m_DiceLevel[_idx] - 1];

	// 올라간 비용 인덱스 반환
	return m_LevelUpSP[++m_DiceLevel[_idx] - 1];
}

void CFieldScript::SetDiceMergeState()
{
	// 병합 가능 상태 설정
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

	// 만약 현재 웨이브가 보스 웨이브일 경우, 모드 매니저로부터 보스 정보를 가져오고 소환.
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
		MessageBox(nullptr, L"파일 오류", L"Deck Info Load 실패", MB_OK);
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
		MessageBox(nullptr, L"파일 오류", L"Deck Info Save 실패", MB_OK);
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
	else // nullptr이 세팅될 경우
	{
		m_FocusMarker->Transform()->SetRelativePos(Vec3(0, 0, 10));

		// 병합 상태 해제
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
	// 필드가 모두 차 있거나, 보유 SP가 소환 SP보다 적을 경우 return
	if (m_CurDiceCount >= 15 || m_SP < m_SummonSP)
		return;

	// SP 소모하기
	m_SP -= m_SummonSP;

	// 소환 SP 증가
	m_SummonSP += 10;


	CDiceScript* pDice = nullptr;

	// 랜덤한 빈 필드 공간 찾기
	while (true)
	{
		pDice = GetRandomFieldDice();
		if (pDice != nullptr && pDice->GetDice() == DICE::NONE)
			break;
	}

	// 나중에는 덱 중에 골라서 넣어주어야 한다.
	pDice->SetDiceType(m_Deck[m_XDis(m_gen)]);

	++m_CurDiceCount;
}

void CFieldScript::SpawnDice(UINT _LOW, UINT _COL)
{
}

#undef OBJECT

