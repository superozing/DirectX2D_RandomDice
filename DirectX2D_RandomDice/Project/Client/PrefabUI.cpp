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
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));

	ImGui::Button(u8"Prefab ���� ������Ʈ");

	ImGui::PopStyleColor(3);
	ImGui::PopID();
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
	// �ش� ������Ʈ�� �����ϰ� �ִ� ������Ʈ�� �����ϴ� ������ƮUI Ȱ��ȭ
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != pInspector->m_arrComUI[i])
		{
			pInspector->m_arrComUI[i]->SetTargetObject(pGameObject);
		}
	}

	// �ش� ������Ʈ�� �����ϰ� �ִ� Script �� ���߾ ScriptUI �� Ȱ��ȭ ��Ų��.
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
