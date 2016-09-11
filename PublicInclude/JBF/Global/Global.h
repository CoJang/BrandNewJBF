#pragma once

#include"Alloc.h"
#include"Hash.h"
#include"Archive.h"
#include"Math/Math.h"

namespace JBF{
    namespace Global{
        extern int MsgBox(
            HWND hWnd,
            LPCTSTR lpCaption,
            UINT uType,
            LPCTSTR lpText,
            ...
        );
    };
};