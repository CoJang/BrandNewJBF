#include"pch.h"
#include"JBF/JBFramework.h"

// Predefinition(s)
///////////////////////////////////////////
#define WINDOW_STYLE WS_OVERLAPPED | WS_CAPTION
///////////////////////////////////////////

namespace JBF{
    // Inner variable(s) definition
    ///////////////////////////////////////////
    static HINSTANCE ins_mainInstance = nullptr;
    static HWND ins_mainHandle = nullptr;
    ///////////////////////////////////////////

    // Inner function(s) definition
    ///////////////////////////////////////////
    static void ins_initApp(const Global::Math::Point<USHORT>* size){
        Core::Input::Init();
        Core::Graphic::Init(size);
        Core::SystemTimer::Init();

        Manager::Stage::Init();
    }
    static void ins_cleanupApp(){
        Manager::Stage::Release();

        Core::Graphic::Release();
        Core::Input::Release();
    }
    static bool ins_updateApp(){
        float delta;

        {
            Core::SystemTimer::Update();
            delta = Core::SystemTimer::GetTimeDelta();
        }

        Core::Input::Update(delta);
        Manager::Stage::Update(delta);

        if(SUCCEEDED(Core::Graphic::CheckState())){
            Manager::Stage::Draw();
            Core::Graphic::Present();
        }

        return true;
    }

    static LRESULT CALLBACK ins_wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        PAINTSTRUCT ps;
        HDC hdc;

        switch (msg){
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return 0;
    }
    ///////////////////////////////////////////

    INLINE HINSTANCE GetInstance(){ return ins_mainInstance; }
    INLINE HWND GetHandle(){ return ins_mainHandle; }

    int RunApp(
        void(*appStartup)(void),
        void(*appEndup)(void),
        LPCTSTR lpszClassName,
        LPCTSTR lpIconName,
        const Global::Math::Point<USHORT>* size
    ){
        ins_mainInstance = GetModuleHandle(nullptr);

        Debug::Dump::Init();

#ifdef _DEBUG
        Debug::Logger::Create(ins_mainInstance);
#endif

        WNDCLASSEX cls = { 0 };
        {
            cls.cbSize = sizeof WNDCLASSEX;
            cls.lpfnWndProc = ins_wndProc;
            cls.cbClsExtra = 0;
            cls.cbWndExtra = 0;
            cls.hInstance = ins_mainInstance;

            cls.hCursor = LoadCursor(ins_mainInstance, IDC_ARROW);
            cls.hIcon = LoadIcon(ins_mainInstance, lpIconName);
            cls.hIconSm = cls.hIcon;
            cls.hIconSm = nullptr;

            cls.style = CS_HREDRAW | CS_VREDRAW;
            cls.hbrBackground = (decltype(cls.hbrBackground))GetStockObject(BLACK_BRUSH);
            cls.lpszClassName = lpszClassName;
            cls.lpszMenuName = nullptr;
        }
        ASSERT(RegisterClassEx(&cls) != 0, _T("Failed to register window."));

        RECT rc = { 0, 0, size->x, size->y };
        AdjustWindowRect(&rc, WINDOW_STYLE, FALSE);

        {
            rc.right -= rc.left;
            rc.left = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) >> 1;
            rc.bottom -= rc.top;
            rc.top = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) >> 1;
        }

        ins_mainHandle = CreateWindow(
            lpszClassName,
            lpszClassName,
            WINDOW_STYLE,
            rc.left,
            rc.top,
            rc.right,
            rc.bottom,
            nullptr,
            nullptr,
            ins_mainInstance,
            nullptr
        );
        ASSERT(ins_mainHandle != nullptr, _T("Failed to create window."));

        ins_initApp(size);
        if (appStartup)appStartup();

        ShowWindow(ins_mainHandle, SW_SHOWDEFAULT);
        SetForegroundWindow(ins_mainHandle);

        MSG msg = { 0 };
        do{
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            if (msg.message == WM_QUIT)break;
        } while (ins_updateApp());

        if (appEndup)appEndup();
        ins_cleanupApp();

#ifdef _DEBUG
        Debug::Logger::Release();
#endif

        Debug::Dump::Release();

        return (int)(msg.wParam);
    }
};