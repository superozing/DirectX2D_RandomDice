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
#include "Content.h"



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

        // 구분선
        ImGui::Separator();

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

        if (ImGui::BeginMenu("Change Object Name", ""))
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

        if (ImGui::BeginMenu("Add Script", ""))
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

                        Outliner* outliner = (Outliner*)CImGuiMgr::GetInst()->FindUI("##Outliner");
                        outliner->ResetCurrentLevel();
                    }
                }
            }

            ImGui::EndMenu();
        }

        // 구분선
        ImGui::Separator();

        // 객체를 Prefab 화
        if (ImGui::BeginMenu("Make Prefab", ""))
        {
            Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");

            // 타겟 오브젝트가 존재할 경우, 이 오브젝트를 프리팹 화 시킨 후 에셋 매니저에 넣어주어야 해요.
            if (nullptr != inspector->GetTargetObject())
            {

                wstring wstrObjName = inspector->GetTargetObject()->GetName();
                string strObjName = ToString(wstrObjName);

                ImGui::Text("New Prefab Name :");
                ImGui::SameLine();

                strObjName.resize(100);

                if (ImGui::InputText("##MakePrefabObjectName", (char*)strObjName.c_str(), 100, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    wstring prefabName = L"prefab\\" + ToWString(strObjName);

                    // 전체 null 제거
                    prefabName.erase(remove(prefabName.begin(), prefabName.end(), '\0'), prefabName.end());
                    prefabName += L".pref";

                    
                    // 프리팹 생성
                    CPrefab* pNewPrefab = new CPrefab;

                    CGameObject* pCloneGameObject = inspector->GetTargetObject()->Clone();
                    
                    pNewPrefab->SetGameObject(pCloneGameObject);
                    pNewPrefab->SetName(prefabName);

                    // 파일에 저장
                    pNewPrefab->Save(prefabName);

                    // 에셋 추가 후 컨텐츠 재구성
                    CAssetMgr::GetInst()->AddAsset<CPrefab>(prefabName, pNewPrefab);
                    
                    Content* content = (Content*)CImGuiMgr::GetInst()->FindUI("##Content");
                    content->ReloadContent();

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
        ImGui::SeparatorText(u8"재질 관련");

        if (ImGui::BeginMenu("Create Empty Material"))
        {
            wstring wstrMtrlName = L"New Material";
            string strMtrlName = ToString(wstrMtrlName);

            ImGui::Text("New Material Name :");
            ImGui::SameLine();

            strMtrlName.resize(100);


            ///
            if (ImGui::InputText("##MakeMaterialName", (char*)strMtrlName.c_str(), 100, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                wstring MtrlName = L"material\\" + ToWString(strMtrlName);

                // 전체 null 제거
                MtrlName.erase(remove(MtrlName.begin(), MtrlName.end(), '\0'), MtrlName.end());
                MtrlName += L".mtrl";

                // 머터리얼 생성
                CMaterial* pMtrl = new CMaterial;
                pMtrl->SetName(MtrlName);

                // 파일에 저장
                pMtrl->Save(MtrlName);

                // 에셋 추가 후 컨텐츠 재구성
                CAssetMgr::GetInst()->AddAsset<CMaterial>(MtrlName, pMtrl);
                Content* content = (Content*)CImGuiMgr::GetInst()->FindUI("##Content");
                content->ReloadContent();
            }

            ImGui::EndMenu();

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



        Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
        Ptr<CAsset> pAsset = pInspector->GetTargetAsset();
        if (pAsset != nullptr && pAsset->GetType() == ASSET_TYPE::PREFAB)
        {
            CPrefab* pPrefab = (CPrefab*)pAsset.Get();

            ImGui::SeparatorText(u8"프리팹 관련");

            if (ImGui::BeginMenu(u8"Prefab 인스턴스화(복제)"))
            {
                string strCloneObjectName;
                ImGui::Text("Instantiate GameObject Name :");
                ImGui::SameLine();
                strCloneObjectName.resize(100);

                ///
                if (ImGui::InputText("##PrefabInstantiateText", (char*)strCloneObjectName.c_str(), 100, ImGuiInputTextFlags_EnterReturnsTrue))
                {

                    // 전체 null 제거
                    strCloneObjectName.erase(remove(strCloneObjectName.begin(), strCloneObjectName.end(), '\0'), strCloneObjectName.end());
                    
                    // 복제
                    CGameObject* pGameObject = pPrefab->Instantiate();

                    pGameObject->SetName(ToWString(strCloneObjectName));
                    GamePlayStatic::SpawnGameObject(pGameObject, 0);
                }

                ImGui::EndMenu();

            }
        }


        ImGui::EndMenu();
    }
}
