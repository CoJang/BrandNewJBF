#pragma once

#include<tchar.h>
#include"DXHeaders.h"
#include"Definitions.h"
#include"Global/Global.h"
#include"Debug/Debug.h"
#include"JBF/Core/Core.h"
#include"Base/Base.h"
#include"Manager/Manager.h"

namespace JBF{
    extern INLINE HINSTANCE GetInstance();
    extern INLINE HWND GetHandle();

    extern int RunApp(
        void(*appStartup)(void),
        void(*appEndup)(void),
        LPCTSTR lpszClassName,
        LPCTSTR lpIconName,
        const Global::Math::Point<USHORT>* size
    );
};

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#define CREATE_MAIN( \
    projectName, \
    appStartup, \
    appEndup, \
    lpIconName, \
    size \
    ) \
    int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR pCmdLine, int nCmdShow){ \
        return JBF::RunApp( \
            appStartup, \
            appEndup, \
            projectName, \
            lpIconName, \
            size \
            ); \
    }
#endif