#include"pch.h"
#include"JBF/Global/Global.h"

namespace JBF{
    namespace Global{
        int MsgBox(
            HWND hWnd,
            LPCTSTR lpCaption,
            UINT uType,
            LPCTSTR lpText,
            ...
        ){
            TCHAR buf[1024];
            va_list iterat;

            va_start(iterat, lpText);
            vstprintf_s(buf, lpText, iterat);
            va_end(iterat);

            buf[_countof(buf) - 1] = 0;

            return MessageBox(hWnd, buf, lpCaption, uType);
        }
    };
};