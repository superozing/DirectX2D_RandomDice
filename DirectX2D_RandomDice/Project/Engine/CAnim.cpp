#include "pch.h"
#include "CAnim.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CAnim::CAnim()
	: m_Animator(nullptr)
	, m_CurFrmIdx(0)
	, m_bFinish(false)
	, m_AccTime(0.f)
{
}

CAnim::CAnim(const CAnim& _OriginAnim)
	: CEntity(_OriginAnim)
	, m_Animator(nullptr)
	, m_vecFrm(_OriginAnim.m_vecFrm)
	, m_CurFrmIdx(_OriginAnim.m_CurFrmIdx)
	, m_bFinish(_OriginAnim.m_bFinish)
	, m_AtlasTex(_OriginAnim.m_AtlasTex)
	, m_AccTime(_OriginAnim.m_AccTime)
{
}

CAnim::~CAnim()
{
}

void CAnim::finaltick()
{
	m_AccTime += DT;

	if (m_vecFrm[m_CurFrmIdx].Duration < m_AccTime)
	{
		++m_CurFrmIdx;
		if (m_vecFrm.size() <= m_CurFrmIdx)
		{
			m_CurFrmIdx = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
		m_AccTime = 0.f;
	}
}

void CAnim::UpdateData()
{
	// 현재 프레임 정보를 상수버퍼로 옮기고 b2 레지스터로 바인딩
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.UseAnim2D = 1;
	data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTop;
	data.vOffset = m_vecFrm[m_CurFrmIdx].vOffset;
	data.vSlizeSize = m_vecFrm[m_CurFrmIdx].vSlice;
	data.vBackGround = m_vecFrm[m_CurFrmIdx].vBackground;

	pCB->SetData(&data);
	pCB->UpdateData();

	// 아틀라스 텍스쳐 t10 에 바인딩
	m_AtlasTex->UpdateData(10);
}

void CAnim::Clear()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.UseAnim2D = 0;

	pCB->SetData(&data);
	pCB->UpdateData();
}

void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop
	, Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount, float _FPS)
{
	m_Animator = _Animator;
	m_AtlasTex = _Atlas;

	for (int i = 0; i < _FrmCount; ++i)
	{
		tAnimFrm frm = {};

		frm.vSlice = Vec2(_vSliceSize.x / (float)_Atlas->GetWidth(), _vSliceSize.y / (float)_Atlas->GetHeight());

		frm.vLeftTop = Vec2(_vLeftTop.x / (float)_Atlas->GetWidth() + frm.vSlice.x * i, _vLeftTop.y / (float)_Atlas->GetHeight());

		frm.vOffset = Vec2(_vOffset.x / (float)_Atlas->GetWidth(), _vOffset.y / (float)_Atlas->GetHeight());
		frm.Duration = 1.f / _FPS;

		frm.vBackground = Vec2(_vBackground.x / (float)_Atlas->GetWidth(), _vBackground.y / (float)_Atlas->GetHeight());


		m_vecFrm.push_back(frm);
	}
}

void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, const vector<tAnimFrm> _vecFrm)
{
	m_Animator = _Animator;
	m_AtlasTex = _Atlas;
	for (int i = 0; i < _vecFrm.size(); ++i)
	{
		tAnimFrm frm = {};
		frm.vSlice = Vec2(_vecFrm[i].vSlice.x / (float)_Atlas->GetWidth(), _vecFrm[i].vSlice.y / (float)_Atlas->GetHeight());
		frm.vLeftTop = Vec2(_vecFrm[i].vLeftTop.x / (float)_Atlas->GetWidth(), _vecFrm[i].vLeftTop.y / (float)_Atlas->GetHeight());
		frm.vOffset = Vec2(_vecFrm[i].vOffset.x / (float)_Atlas->GetWidth(), _vecFrm[i].vOffset.y / (float)_Atlas->GetHeight());
		frm.Duration = _vecFrm[i].Duration;
		frm.vBackground = Vec2(_vecFrm[i].vBackground.x / (float)_Atlas->GetWidth(), _vecFrm[i].vBackground.y / (float)_Atlas->GetHeight());

		m_vecFrm.push_back(frm);
	}
}

void CAnim::CreateFrm(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, vector<tAnimFrm> _vecAnimFrm)
{
	m_Animator = _Animator;
	m_AtlasTex = _Atlas;

	for (int i = 0; i < (int)_vecAnimFrm.size(); ++i)
	{
		tAnimFrm frm = {};

		frm.vSlice = Vec2(_vecAnimFrm[i].vSlice.x / (float)_Atlas->GetWidth(), _vecAnimFrm[i].vSlice.y / (float)_Atlas->GetHeight());

		frm.vLeftTop = Vec2(_vecAnimFrm[i].vLeftTop.x / (float)_Atlas->GetWidth(),
			_vecAnimFrm[i].vLeftTop.y / (float)_Atlas->GetHeight());

		frm.vOffset = Vec2(_vecAnimFrm[i].vOffset.x / (float)_Atlas->GetWidth(), _vecAnimFrm[i].vOffset.y / (float)_Atlas->GetHeight());

		frm.Duration = _vecAnimFrm[i].Duration;

		frm.vBackground = Vec2(_vecAnimFrm[i].vBackground.x / (float)_Atlas->GetWidth(),
			_vecAnimFrm[i].vBackground.y / (float)_Atlas->GetHeight());

		m_vecFrm.push_back(frm);
	}
}

bool CAnim::Save(const wstring& _FilePath, const wstring& _strKey)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"w");

	if (nullptr == pFile)
	{
		MessageBox(nullptr, L"Animation Save 실패", L"Animation Save 실패", MB_OK);
		return false;
	}

	// Animation 이름 저장
	fwprintf_s(pFile, L"[ANIM_NAME]\n");

	wstring strName = _strKey;
	fwprintf_s(pFile, strName.c_str());
	fwprintf_s(pFile, L"\n\n");


	// 키, 경로
	fwprintf_s(pFile, L"[ATLAS_TEXTURE]\n");

	wstring strKey;
	wstring strRelativePath;

	if (m_AtlasTex != nullptr)
	{
		strKey = m_AtlasTex->GetKey();
		strRelativePath = m_AtlasTex->GetRelativePath();
	}

	fwprintf_s(pFile, strKey.c_str());
	fwprintf_s(pFile, L"\n");
	fwprintf_s(pFile, strRelativePath.c_str());
	fwprintf_s(pFile, L"\n\n");

	// 프레임 데이터
	// 프레임 숫자
	fwprintf_s(pFile, L"[FRAME_COUNT]\n");
	fwprintf_s(pFile, L"%d\n\n", (int)m_vecFrm.size());


	for (int i = 0; i < (int)m_vecFrm.size(); ++i)
	{
		fwprintf_s(pFile, L"[FRAME_NUM]\n");
		fwprintf_s(pFile, L"%d\n", i);

		fwprintf_s(pFile, L"[LEFT_TOP]\n");
		fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].vLeftTop.x, m_vecFrm[i].vLeftTop.y);

		fwprintf_s(pFile, L"[CUT_SIZE]\n");
		fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].vSlice.x, m_vecFrm[i].vSlice.y);

		fwprintf_s(pFile, L"[OFFSET]\n");
		fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].vOffset.x, m_vecFrm[i].vOffset.y);

		fwprintf_s(pFile, L"[BACKGROUND]\n");
		fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].vBackground.x, m_vecFrm[i].vBackground.y);

		fwprintf_s(pFile, L"[DURATION]\n");
		fwprintf_s(pFile, L"%f\n\n", m_vecFrm[i].Duration);
	}

	fclose(pFile);

	return true;
}

bool CAnim::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"r");

	if (nullptr == pFile)
	{
		MessageBox(nullptr, L"Animation Load 실패", L"Animation Load 실패", MB_OK);
		return false;
	}

	// Animation 이름 로드

	while (true)
	{
		wchar_t szRead[256] = {};
		if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
		{
			break;
		}

		if (!wcscmp(szRead, L"[ANIM_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szRead, 256);
			SetName(szRead);
		}
		else if (!wcscmp(szRead, L"[ATLAS_TEXTURE]"))
		{
			wstring strKey, strRelativePath;

			fwscanf_s(pFile, L"%s", szRead, 256);
			strKey = szRead;

			fwscanf_s(pFile, L"%s", szRead, 256);
			strRelativePath = szRead;

			m_AtlasTex = CAssetMgr::GetInst()->Load<CTexture>(strKey, strRelativePath);
		}
		else if (!wcscmp(szRead, L"[FRAME_COUNT]"))
		{
			int iFrameCount = 0;
			fwscanf_s(pFile, L"%d", &iFrameCount);
			m_vecFrm.resize(iFrameCount);

			int iCurFrame = 0;
			while (true)
			{
				fwscanf_s(pFile, L"%s", szRead, 256);

				if (!wcscmp(szRead, L"[FRAME_NUM]"))
				{
					fwscanf_s(pFile, L"%d", &iCurFrame);
				}
				else if (!wcscmp(szRead, L"[LEFT_TOP]"))
				{
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vLeftTop.x);
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vLeftTop.y);
				}
				else if (!wcscmp(szRead, L"[CUT_SIZE]"))
				{
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vSlice.x);
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vSlice.y);
				}
				else if (!wcscmp(szRead, L"[OFFSET]"))
				{
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vOffset.x);
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vOffset.y);
				}
				else if (!wcscmp(szRead, L"[BACKGROUND]"))
				{
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vBackground.x);
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vBackground.y);
				}
				else if (!wcscmp(szRead, L"[DURATION]"))
				{
					fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].Duration);

					if (iFrameCount - 1 <= iCurFrame)
						break;
				}
			}
		}
	}

	fclose(pFile);
	return true;
}

void CAnim::SaveToFile(FILE* _File)
{
	// 애니메이션 이름 저장
	SaveWString(GetName(), _File);

	// 모든 프레임 정보 저장
	size_t FrmSize = m_vecFrm.size();
	fwrite(&FrmSize, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

	// 애니메이션이 참조하던 텍스쳐 정보 저장
	SaveAssetRef(m_AtlasTex, _File);
}

void CAnim::LoadFromFile(FILE* _File)
{
	// 애니메이션 이름 로드
	wstring strName;
	LoadWString(strName, _File);
	SetName(strName);

	// 모든 프레임 정보 로드
	size_t FrmSize = 0;
	fread(&FrmSize, sizeof(size_t), 1, _File);
	m_vecFrm.resize(FrmSize);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

	// 애니메이션이 참조하던 텍스쳐 정보 로드
	LoadAssetRef(m_AtlasTex, _File);
}
