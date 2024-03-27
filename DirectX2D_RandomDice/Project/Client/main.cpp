#include "pch.h"
#include "framework.h"
#include "Client.h"

#include <crtdbg.h>
#include <dwmapi.h>

#include <Engine\global.h>
#include <Engine\CEngine.h>
#include <Engine\CDevice.h>
#include <Engine/CPrefab.h>
#include "CLevelSaveLoad.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_d.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Scripts\\Scripts_d.lib")
#else
#pragma comment(lib, "Scripts\\Scripts.lib")
#endif


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "CImGuiMgr.h"
#include "CEditorObjMgr.h"
#include "CCreateTempLevel.h"


HINSTANCE   hInst;
HWND        hWnd;

static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

// 전역 해상도
Vec2        Resolution;


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // memory lick check
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(136);

    MyRegisterClass(hInstance);

    // ------------------
    // Init
    // ------------------
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
    MSG msg;

#ifdef _DEBUG
    RECT rect = { 0,0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top - 79, 0);
    GetClientRect(hWnd, &rect);

    // window size, position
    Vec2 WinSize = Vec2((float)rect.right, (float)rect.bottom);
    rect = { 0, 0, (int)WinSize.x, (int)WinSize.y };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(hWnd, nullptr, -10, 0, rect.right - rect.left, rect.bottom - rect.top, 0);
#else
    Vec2 WinSize = Vec2(1600, 900);
    LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
    style &= ~WS_OVERLAPPEDWINDOW;
    style |= WS_POPUP;
    SetWindowLongPtr(hWnd, GWL_STYLE, style);
    SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED | SWP_SHOWWINDOW);
#endif

    // window style
    COLORREF DARK_COLOR = 0x00151515;
    BOOL SET_BORDER_COLOR = SUCCEEDED(DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &DARK_COLOR, sizeof(DARK_COLOR)));
    BOOL SET_CAPTION_COLOR = SUCCEEDED(DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &DARK_COLOR, sizeof(DARK_COLOR)));

    // CEnigne init
    if (FAILED(CEngine::GetInst()->init(hWnd, WinSize)))
    {
        MessageBox(nullptr, L"Failed to initialize CEngine", L"Faile to initialize", MB_OK);
        return 0;
    }

    CPrefab::GAMEOBJECT_SAVE = &CLevelSaveLoad::SaveGameObject;
    CPrefab::GAMEOBJECT_LOAD = &CLevelSaveLoad::LoadGameObject;

#ifndef _RELEASE_GAME
    // create temp level
    CCreateTempLevel::Init();
    CCreateTempLevel::CreateTempLevel();

    // EditorObjectManager 초기화
    CEditorObjMgr::GetInst()->init();

    // ImGUI init
    CImGuiMgr::GetInst()->init(hWnd, DEVICE, CONTEXT);

#endif

    // ------------------
    // main loop
    // ------------------
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // update
            CEngine::GetInst()->progress();

#ifndef _RELEASE_GAME
            CEditorObjMgr::GetInst()->progress();
            CImGuiMgr::GetInst()->progress();
#endif

            // Engine + ImGUI의 렌더링 최종 결과 present
            CDevice::GetInst()->Present();
        }
    }

    return (int)msg.wParam;
}


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr; //MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = L"MyWindow";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(L"MyWindow", L"Client", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;   
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
