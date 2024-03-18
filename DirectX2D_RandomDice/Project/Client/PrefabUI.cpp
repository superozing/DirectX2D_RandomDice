#include "pch.h"
#include "PrefabUI.h"
#include "Inspector.h"
#include "ComponentUI.h"
#include "ScriptUI.h"

PrefabUI::PrefabUI()
	: AssetUI("Prefab", "##Prefab", ASSET_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::render_update()
{
	AssetUI::render_update();
}

void PrefabUI::Activate()
{
	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");

	Ptr<CAsset> pAsset = pInspector->GetTargetAsset();

	CPrefab* pPrefab = nullptr;
	CGameObject* pGameObject = nullptr;

	if (pAsset->GetType() == ASSET_TYPE::PREFAB)
	{
		pPrefab = (CPrefab*)pAsset.Get();
		pGameObject = pPrefab->GetProtoObj();
	}
	// 해당 오브젝트가 보유하고 있는 컴포넌트에 대응하는 컴포넌트UI 활성화
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != pInspector->m_arrComUI[i])
		{
			pInspector->m_arrComUI[i]->SetTargetObject(pGameObject);
		}
	}

	// 해당 오브젝트가 보유하고 있는 Script 에 맞추어서 ScriptUI 를 활성화 시킨다.
	for (size_t i = 0; i < pInspector->m_vecScriptUI.size(); ++i)
	{
		pInspector->m_vecScriptUI[i]->Deactivate();
	}

	if (nullptr != pGameObject)
	{
		if (pInspector->m_vecScriptUI.size() < pGameObject->GetScripts().size())
		{
			pInspector->ResizeScriptUI(pGameObject->GetScripts().size());
		}

		const vector<CScript*>& vecScripts = pGameObject->GetScripts();
		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			pInspector->m_vecScriptUI[i]->SetScript(vecScripts[i]);
		}
	}
}
