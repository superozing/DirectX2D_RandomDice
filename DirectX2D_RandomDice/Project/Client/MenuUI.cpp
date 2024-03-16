#include "pch.h"
#include "MenuUI.h"



#include <Engine/CPathMgr.h>
#include <Engine/CTaskMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Scripts/CScriptMgr.h>
#include <Engine/CScript.h>

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "Outliner.h"
#include "CLevelSaveLoad.h"



MenuUI::MenuUI()
	: UI("Menu", "##Menu")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::render()
{
    if (ImGui::BeginMainMenuBar())
    {     
        render_update();

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::render_update()
{
    File();

    Level();

    GameObject();
    
    Asset();
}

void MenuUI::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Save Level", ""))
        {
            wchar_t szSelect[256] = {};

            OPENFILENAME ofn = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정
            wstring strInitPath = CPathMgr::GetContentPath();
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetSaveFileName(&ofn))
            {                
                CLevelSaveLoad::SaveLevel(CLevelMgr::GetInst()->GetCurrentLevel(), CPathMgr::GetRelativePath(szSelect));
            }
        }

        if (ImGui::MenuItem("Load Level", ""))
        {
            wchar_t szSelect[256] = {};

            OPENFILENAME ofn = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정
            wstring strInitPath = CPathMgr::GetContentPath();
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn))
            {                
                CLevel* pLevel = CLevelSaveLoad::LoadLevel(CPathMgr::GetRelativePath(szSelect));
                CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);

                // Inspector 의 타겟정보를 nullptr 로 되돌리기
                Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
                pInspector->SetTargetObject(nullptr);
            }
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Level()
{
    if (ImGui::BeginMenu("Level"))
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

        LEVEL_STATE State = pCurLevel->GetState();

        bool PlayEnable = false;
        bool PauseEnable = false;
        bool StopEnable = false;

        if (LEVEL_STATE::STOP == State || LEVEL_STATE::PAUSE == State)
            PlayEnable = true;
        else
            PlayEnable = false;

        if (LEVEL_STATE::PLAY == State)
            PauseEnable = true;
        else
            PauseEnable = false;

        if (LEVEL_STATE::PLAY == State || LEVEL_STATE::PAUSE == State)
            StopEnable = true;
        else
            StopEnable = false;

        
        if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
        {   
            if (LEVEL_STATE::STOP == pCurLevel->GetState())
            {
                CLevelSaveLoad::SaveLevel(pCurLevel, L"Level//temp.lv");
            }
            
            CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PLAY);
        }

        if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
        {
            CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PAUSE);
        }

        if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
        {
            CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"Level//temp.lv");
            CLevelMgr::GetInst()->ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

            // Inspector 의 타겟정보를 nullptr 로 되돌리기
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
            pInspector->SetTargetObject(nullptr);
        }

        ImGui::EndMenu();
    }
}

void MenuUI::GameObject()
{
    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Create Empty Object", ""))
        {
            CGameObject* pNewObj = new CGameObject;
            pNewObj->SetName(L"New GameObject");
            pNewObj->AddComponent(new CTransform);
            GamePlayStatic::SpawnGameObject(pNewObj, 0);
        }
        ImGui::Separator();

        if (ImGui::BeginMenu("Component", ""))
        {
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add Component", ""))
        {
            Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");

            if (nullptr != inspector->GetTargetObject())
            {
                if (inspector->GetTargetObject()->Transform() == nullptr)
                    if (ImGui::MenuItem("Transform")) inspector->GetTargetObject()->AddComponent(new CTransform);

                if (inspector->GetTargetObject()->Collider2D() == nullptr)
                    if (ImGui::MenuItem("Collider2D")) inspector->GetTargetObject()->AddComponent(new CCollider2D);

                if (inspector->GetTargetObject()->Animator2D() == nullptr)
                    if (ImGui::MenuItem("Animator2D")) inspector->GetTargetObject()->AddComponent(new CAnimator2D);

                if (inspector->GetTargetObject()->Light2D() == nullptr)
                    if (ImGui::MenuItem("Light2D")) inspector->GetTargetObject()->AddComponent(new CLight2D);

                if (inspector->GetTargetObject()->StateMachine() == nullptr)
                    if (ImGui::MenuItem("StateMachine")) inspector->GetTargetObject()->AddComponent(new CStateMachine);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add Render Component", ""))
        {
            Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");

            if (nullptr != inspector->GetTargetObject() && inspector->GetTargetObject()->GetRenderComopnent() == nullptr)
            {
                if (ImGui::MenuItem("MeshRender"))
                    inspector->GetTargetObject()->AddComponent(new CMeshRender);

                if (ImGui::MenuItem("TileMap"))
                    inspector->GetTargetObject()->AddComponent(new CTileMap);

                if (ImGui::MenuItem("ParticleSystem"))
                    inspector->GetTargetObject()->AddComponent(new CParticleSystem);

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Change Name", ""))
        {
            Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");

            if (nullptr != inspector->GetTargetObject())
            {
                
                wstring wstrObjName = inspector->GetTargetObject()->GetName();
                string strObjName = ToString(wstrObjName);

                ImGui::Text("Name ");
                ImGui::SameLine();
                if (ImGui::InputText("##ObjName", (char*)strObjName.c_str(), 100, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    inspector->GetTargetObject()->SetName(ToWString(strObjName));
                    Outliner* outliner = (Outliner*)CImGuiMgr::GetInst()->FindUI("##Outliner");
                    outliner->ResetCurrentLevel();
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Script", ""))
        {
            vector<wstring> vecScriptName;
            CScriptMgr::GetScriptInfo(vecScriptName);

            for (size_t i = 0; i < vecScriptName.size(); ++i)
            {
                if (ImGui::MenuItem(ToString(vecScriptName[i]).c_str()))
                {
                    Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
                    if (nullptr != inspector->GetTargetObject())
                    {                     
                        inspector->GetTargetObject()->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));
                    }
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        if (ImGui::MenuItem("Create Empty Material"))
        {
            wchar_t szPath[255] = {};            
            wstring FilePath = CPathMgr::GetContentPath();
            
            int num = 0;
            while (true)
            {                
                swprintf_s(szPath, L"Material//New Material_%d.mtrl", num);
                if (!exists(FilePath + szPath))
                    break;
                ++num;
            }

            CMaterial* pMtrl = new CMaterial;
            pMtrl->SetName(szPath);
            pMtrl->Save(szPath);
            GamePlayStatic::AddAsset(pMtrl);
        }

        Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
        CMaterial* target = dynamic_cast<CMaterial*>(inspector->GetTargetAsset().Get());
        if (target != nullptr)
        {
            if (ImGui::MenuItem("Add Texture Param"))
            {
                if (target->GetTexParam(TEX_PARAM::TEX_0) == nullptr)
                    target->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\Dice\\03_wind.png"));
            }
        }

        ImGui::EndMenu();
    }
}
