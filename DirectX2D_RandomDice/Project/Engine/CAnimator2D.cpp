#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_CurAnim(nullptr)
	, m_bRepeat(true)
{

}

CAnimator2D::CAnimator2D(const CAnimator2D& _OriginAnimator)
	: CComponent(_OriginAnimator)
	, m_CurAnim(nullptr)
	, m_bRepeat(_OriginAnimator.m_bRepeat)
{
	map<wstring, CAnim*>::const_iterator iter = _OriginAnimator.m_mapAnim.begin();
	for (; iter != _OriginAnimator.m_mapAnim.end(); ++iter)
	{
		CAnim* pCloneAnim = iter->second->Clone();

		pCloneAnim->m_Animator = this;
		m_mapAnim.insert(make_pair(iter->first, pCloneAnim));
	}

	if (nullptr != _OriginAnimator.m_CurAnim)
	{
		m_CurAnim = FindAnim(_OriginAnimator.m_CurAnim->GetName());
	}
}

CAnimator2D::~CAnimator2D()
{
	Delete_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr == m_CurAnim)
		return;

	if (m_CurAnim->IsFinish() && m_bRepeat)
	{
		m_CurAnim->Reset();
	}

	m_CurAnim->finaltick();
}

void CAnimator2D::UpdateData()
{
	if (nullptr == m_CurAnim)
		return;

	m_CurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
	CAnim::Clear();
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, Vec2 _LeftTop
	, Vec2 _vSliceSize, Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _FPS)
{
	CAnim* pAnim = FindAnim(_strKey);
	assert(!pAnim);


	pAnim = new CAnim;
	pAnim->Create(this, _AltasTex, _LeftTop, _vSliceSize, _OffsetSize, _Background, _FrmCount, _FPS);
	pAnim->SetName(_strKey);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

void CAnimator2D::Create(const wstring& _strKey, CAnim* _Anim)
{
	CAnim* pAnim = FindAnim(_strKey);
	assert(!pAnim);
	_Anim->SetName(_strKey);
	m_mapAnim.insert(make_pair(_strKey, _Anim));

}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, const vector<tAnimFrm>& _vecFrm)
{
	CAnim* pAnim = FindAnim(_strKey);
	if (pAnim && _strKey == L"PreviewAnim")
	{
		m_mapAnim.erase(m_mapAnim.find(_strKey));
		pAnim->Clear();
	}
	else
		assert(!pAnim);

	pAnim = new CAnim;
	pAnim->Create(this, _AltasTex, _vecFrm);
	pAnim->SetName(_strKey);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}
void CAnimator2D::AnimDelete(const wstring& _strKey)
{
	//map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strKey);
	//delete iter->second;
	m_mapAnim.erase(_strKey);
}

void CAnimator2D::CreateFrm(const wstring& _strKey, Ptr<CTexture> _AtlasTex, vector<tAnimFrm> _vecAnimFrm)
{
	CAnim* pAnim = FindAnim(_strKey);

	assert(!pAnim);

	pAnim = new CAnim;
	pAnim->CreateFrm(this, _AtlasTex, _vecAnimFrm);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

CAnim* CAnimator2D::FindAnim(const wstring& _strKey)
{
	map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _bRepeat)
{
	CAnim* pAnim = FindAnim(_strAnimName);
	if (nullptr == pAnim)
		return;

	m_bRepeat = _bRepeat;

	m_CurAnim = pAnim;
	//m_CurAnim->Reset();
}

void CAnimator2D::Stop()
{
	m_CurAnim->Reset();
	m_CurAnim = nullptr;
}


void CAnimator2D::SaveAnimations(const wstring& _strRelativePath)
{
	wstring strFolderPath = CPathMgr::GetContentPath();
	strFolderPath += _strRelativePath;

	for (const auto& pair : m_mapAnim)
	{
		wstring strFilePath = strFolderPath + L"\\" + pair.first + L".txt";
		wstring strKey = pair.first;
		if (!pair.second->Save(strFilePath, strKey))
		{
			MessageBox(nullptr, L"Animation 생성 실패", L"Animation 생성 실패", MB_OK);
		}
	}
}

void CAnimator2D::LoadAnimation(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	CAnim* pNewAnim = new CAnim;

	if (!pNewAnim->Load(strFilePath))
	{
		MessageBox(nullptr, L"Animation Load 실패", L"Animation Load 실패", MB_OK);
		delete pNewAnim;
		return;
	}

	pNewAnim->m_Animator = this;
	m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
}

map<wstring, CAnim*> CAnimator2D::GetAnimList()
{
	return m_mapAnim;
}

void CAnimator2D::SaveToFile(FILE* _File)
{
	// 애니메이션 개수 저장
	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _File);

	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToFile(_File);
	}

	// 플레이 중이던 애니메이션의 키를 저장한다.
	wstring PlayAnimName;

	if (nullptr != m_CurAnim)
	{
		PlayAnimName = m_CurAnim->GetName();
	}

	SaveWString(PlayAnimName, _File);
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);
}

void CAnimator2D::LoadFromFile(FILE* _File)
{
	// 애니메이션 개수 로드
	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < AnimCount; ++i)
	{
		CAnim* pAnim = new CAnim;
		pAnim->LoadFromFile(_File);

		pAnim->m_Animator = this;
		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
	}

	// 플레이 중이던 애니메이션의 키를 불러온다
	wstring PlayAnimName;
	LoadWString(PlayAnimName, _File);

	if (!PlayAnimName.empty())
	{
		m_CurAnim = FindAnim(PlayAnimName);
	}

	fread(&m_bRepeat, sizeof(bool), 1, _File);
}
