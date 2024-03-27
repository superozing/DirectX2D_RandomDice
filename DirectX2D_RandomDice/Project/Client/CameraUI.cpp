#include "pch.h"
#include "CameraUI.h"

#include <Engine\CTaskMgr.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>

#include <Engine\CCamera.h>


#include "CImGuiMgr.h"
#include "ListUI.h"

CameraUI::CameraUI()
	: ComponentUI("Camera", "##Camera", COMPONENT_TYPE::CAMERA)
{
	//SetSize(ImVec2(0.f, 140.f));
	SetComopnentTitle("Camera");



}

CameraUI::~CameraUI()
{
}

void CameraUI::CheckLayer(DWORD_PTR _ptr)
{

	if (nullptr == GetTargetObject() || nullptr == GetTargetObject()->Camera())
		return;

	string strState = (char*)_ptr;

	int LayerIdx = -1;

	size_t start_pos = strState.find("[") + 1;
	size_t end_pos = strState.find("]");
	if (start_pos != std::string::npos && end_pos != std::string::npos) {
		std::string num_str = strState.substr(start_pos, end_pos - start_pos);
		LayerIdx = std::stoi(num_str);
	}

	GetTargetObject()->Camera()->LayerCheck(LayerIdx, true);

}

void CameraUI::render_update()
{

	// Layer �̸��� CImGuiMgr ���Լ� ������
	m_vecLayerName = CImGuiMgr::GetInst()->GetLayerName();


	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();

	CCamera* pCamera = GetTargetObject()->Camera();

	// Priority
	ImGui::Text("Camera Priority"); ImGui::SameLine(150);
	int Priority = pCamera->GetPriority();
	ImGui::InputInt("##Priority", &Priority);

	if (Priority >= 0)
	{
		pCamera->SetCameraPriority(Priority);
	}

	// Projection Type ���� ��ӹڽ�
	PROJ_TYPE CurType = pCamera->GetProjType();
	string ProjType = "";

	switch (CurType)
	{
	case PROJ_TYPE::ORTHOGRAPHIC:
		ProjType = "ORTHOGRAPHIC";
		break;
	case PROJ_TYPE::PERSPECTIVE:
		ProjType = "PERSPECTIVE";
		break;
	default:
		break;
	}

	ImGui::Text("Projection Type"); ImGui::SameLine(150);

	if (ImGui::BeginCombo("##ProJecTypeCombo", ProjType.data())) {
		if (ImGui::Selectable("ORTHOGRAPHIC"))
		{
			pCamera->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		}
		if (ImGui::Selectable("PERSPECTIVE"))
		{
			pCamera->SetProjType(PROJ_TYPE::PERSPECTIVE);
		}

		ImGui::EndCombo();
	}

	if (CurType == PROJ_TYPE::PERSPECTIVE)
	{
		// FOV
		ImGui::Text("FOV"); ImGui::SameLine(150);
		float FOV = pCamera->GetFOV();
		ImGui::DragFloat("##CameraFOV", &FOV, 0.1, 0.1f, 3.14f);
		pCamera->SetFOV(FOV);
	}
	else
	{
		// Scale
		ImGui::Text("Scale"); ImGui::SameLine(150);
		float Scale = pCamera->GetScale();
		ImGui::DragFloat("##CameraScale", &Scale);
		if (Scale != 0.f)
		{
			pCamera->SetScale(Scale);
		}
	}


	// Far
	ImGui::Text("Far"); ImGui::SameLine(150);
	float Far = pCamera->GetFar();
	ImGui::DragFloat("##CameraFar", &Far);
	if (Far > 1.f)
	{
		pCamera->SetFar(Far);
	}

	//UINT                    m_LayerCheck;

	vector<string> items;
	vector<string> NoneCheckLayer;

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		if (pCamera->IsLayerCheck(i))
		{
			items.push_back(m_vecLayerName[i]);
		}
		else
		{
			NoneCheckLayer.push_back(m_vecLayerName[i]);
		}
	}

	static int item_current_idx = 0; // Here we store our selection data as an index.
	if (ImGui::BeginListBox("##CheckLayerList", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < items.size(); i++)
		{
			const bool is_selected = (item_current_idx == i);

			if (ImGui::Selectable(items[i].c_str(), is_selected))
				item_current_idx = i;

			// ����Ʈ �� �ش� �׸��� Ŭ���Ǹ� ���̶���Ʈ �ɾ���
			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}
		ImGui::EndListBox();
	}


	// ��ư �ʺ� ����
	float buttonWidth = 150.0f;

	// ��ư�� ������ �����Ͽ� ��ġ
	float posX = ImGui::GetWindowWidth() - buttonWidth * 2 - ImGui::GetStyle().ItemSpacing.x;
	ImGui::SetCursorPosX(posX);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0)); // ��ư �� ���� ����

	if (ImGui::Button("Check", ImVec2(buttonWidth, 0)))
	{
		// ����Ʈ UI
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

		pListUI->AddString(NoneCheckLayer);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&CameraUI::CheckLayer);
		pListUI->Activate();
	}

	ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.x);

	if (ImGui::Button("UnCheck", ImVec2(buttonWidth, 0)))
	{
		if (item_current_idx < items.size())
		{
			string TargetLayer = items[item_current_idx];
			int LayerIdx = -1;

			size_t start_pos = TargetLayer.find("[") + 1;
			size_t end_pos = TargetLayer.find("]");
			if (start_pos != std::string::npos && end_pos != std::string::npos) {
				std::string num_str = TargetLayer.substr(start_pos, end_pos - start_pos);
				LayerIdx = std::stoi(num_str);
			}

			pCamera->LayerCheck(LayerIdx, false);
		}
	}
	ImGui::PopStyleVar();




	m_vecLayerName.clear();
}