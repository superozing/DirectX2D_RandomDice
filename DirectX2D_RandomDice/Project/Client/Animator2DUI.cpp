#include "pch.h"
#include "Animator2DUI.h"

#include "EditAnimator.h"

Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D", "##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
	SetSize(ImVec2(0.f, 100.f));
	//SetComponentTitle("Animator2D");
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::render_update()
{
	ComponentUI::render_update();

	if (ImGui::Button("Create Animation", ImVec2(130, 20)))
	{
		EditAnimator* pEdit = (EditAnimator*)CImGuiMgr::GetInst()->FindUI("##EditAnimation");
		CGameObject* pTarget = GetTargetObject();
		pEdit->SetTaretObject(pTarget);
		pEdit->Activate();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Animation", ImVec2(130, 20)))
	{
		OpenFildDialog();
	}

	map<wstring, CAnim*> animList = GetTargetObject()->Animator2D()->GetAnimList();

	static vector<string> animNames;

	if (!animList.empty())
	{
		for (const auto& pair : animList)
		{
			wstring animName = pair.first;
			string animNameStr = ToString(animName);

			// 중복 체크
			if (std::find(animNames.begin(), animNames.end(), animNameStr) == animNames.end())
			{
				animNames.push_back(animNameStr);
			}
		}

		static int selectedIndex = 0;
		ImGui::PushItemWidth(150);
		if (ImGui::BeginCombo("##AnimList", animNames[selectedIndex].c_str()))
		{
			for (int i = 0; i < (int)animNames.size(); ++i)
			{
				bool isSelected = (selectedIndex == i);

				if (ImGui::Selectable(animNames[i].c_str(), isSelected))
				{
					selectedIndex = i;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::Button("Play Animation"))
		{
			if (selectedIndex >= 0 && selectedIndex < animNames.size())
			{
				string selectedAnimName = animNames[selectedIndex];
				wstring selectedAnimNameW = ToWString(selectedAnimName);

				GetTargetObject()->Animator2D()->Play(selectedAnimNameW);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Stop Animation"))
		{
			if (selectedIndex >= 0 && selectedIndex < animNames.size())
			{
				GetTargetObject()->Animator2D()->Stop();
			}
		}
	}
}

void Animator2DUI::OpenFildDialog()
{
	wchar_t szFile[MAX_PATH] = L"";
	//OPENFILENAME ofn;

	//ZeroMemory(&ofn, sizeof(ofn));
	//ofn.lStructSize = sizeof(ofn);
	//ofn.hwndOwner = NULL;
	//ofn.lpstrFile = szFile;
	//ofn.lpstrFile[0] = '\0';
	//ofn.nMaxFile = sizeof(szFile) / sizeof(*szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0";
	//ofn.nFilterIndex = 1;
	//ofn.lpstrFileTitle = NULL;
	//ofn.nMaxFileTitle = 0;
	//ofn.lpstrInitialDir = NULL;
	//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"ALL\0*.*\0Animdata\0*.lv";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	// 탐색창 초기 위치 지정
	wstring strInitPath = CPathMgr::GetContentPath();
	strInitPath += L"animdata\\";
	ofn.lpstrInitialDir = strInitPath.c_str();

	if (GetOpenFileName(&ofn))
	{
		const wstring filePath = szFile;
		LoadAnimationFromFile(filePath);
	}
}


void Animator2DUI::LoadAnimationFromFile(const wstring& filePath)
{
	wstring fileName = GetFileFromPath(filePath);

	GetTargetObject()->Animator2D()->LoadAnimation(fileName);
}

wstring Animator2DUI::GetFileFromPath(const wstring& filePath)
{
	size_t found = filePath.find(L"content\\");

	if (found != wstring::npos)
	{
		return filePath.substr(found + 8);
	}

	return filePath;
}