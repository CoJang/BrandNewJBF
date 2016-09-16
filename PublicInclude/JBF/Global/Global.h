#pragma once

#include"Alloc.h"
#include"Hash.h"
#include"Archive.h"
#include"Math/Math.h"

namespace JBF{
    namespace Global{
        extern int MsgBox(
            HWND hWnd,
            LPCSTR lpCaption,
            UINT uType,
            LPCSTR lpText,
            ...
        );
        extern int MsgBox(
            HWND hWnd,
            LPCWSTR lpCaption,
            UINT uType,
            LPCWSTR lpText,
            ...
        );
    };
};