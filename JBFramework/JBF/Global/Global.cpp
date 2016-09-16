#include"pch.h"
#include"JBF/Global/Global.h"

namespace JBF{
    namespace Global{
        int MsgBox(
            HWND hWnd,
            LPCSTR lpCaption,
            UINT uType,
            LPCSTR lpText,
            ...
        ){
            CHAR buf[1024];
            va_list iterat;

            va_start(iterat, lpText);
            vsprintf_s(buf, lpText, iterat);
            va_end(iterat);

            buf[_countof(buf) - 1] = 0;

            return MessageBoxA(hWnd, buf, lpCaption, uType);
        }
        int MsgBox(
            HWND hWnd,
            LPCWSTR lpCaption,
            UINT uType,
            LPCWSTR lpText,
            ...
        ){
            WCHAR buf[1024];
            va_list iterat;

            va_start(iterat, lpText);
            vswprintf_s(buf, lpText, iterat);
            va_end(iterat);

            buf[_countof(buf) - 1] = 0;

            return MessageBoxW(hWnd, buf, lpCaption, uType);
        }
    };
};