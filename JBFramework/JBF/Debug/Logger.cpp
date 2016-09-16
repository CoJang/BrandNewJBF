#include"pch.h"

#ifdef _DEBUG
#include<commctrl.h>
#include<mutex>
#include<vector>
#include<list>
#include<string>
#include"JBF/Debug/Debug.h"

#pragma comment(lib,"comctl32.lib")

// Predefinition(s)
///////////////////////////////////////////
#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)

#define PRIMITIVE_ASSERTW(isFalse, ...) \
    if(!(bool)(isFalse)){ \
        JBF::Global::MsgBox( \
            nullptr, \
            L"Assertion failed", \
            MB_ICONERROR | MB_OK, \
            L"Assertion failed!\n\n" L"File: " _T(__FILE__) L"\n" L"Line: " STRINGIFY_BUILTIN(__LINE__) L"\n\n" L"Expression: " STRINGIFY_BUILTIN(isFalse) L"\n\n%s", \
            __VA_ARGS__ \
            ); \
        __debugbreak(); \
    }
#define PRIMITIVE_ASSERTA(isFalse, ...) \
    if(!(bool)(isFalse)){ \
        JBF::Global::MsgBox( \
            nullptr, \
            "Assertion failed", \
            MB_ICONERROR | MB_OK, \
            "Assertion failed!\n\n" "File: " _T(__FILE__) "\n" "Line: " STRINGIFY_BUILTIN(__LINE__) "\n\n" "Expression: " STRINGIFY_BUILTIN(isFalse) "\n\n%s" \
            __VA_ARGS__ \
            ); \
        __debugbreak(); \
    }
#ifdef UNICODE
#define PRIMITIVE_ASSERT(isFalse, ...) PRIMITIVE_ASSERTW(isFalse, __VA_ARGS__)
#else
#define PRIMITIVE_ASSERT(isFalse, ...) PRIMITIVE_ASSERTA(isFalse, __VA_ARGS__)
#endif
 

#define IDC_LISTSTR 101
#define IDC_LISTNUM 102

#define SIZE_LISTSTR_X 270
#define SIZE_LISTNUM_X 30
#define SIZE_LIST_Y 500

#define FONT_NAME _T("Courier New")
#define FONT_SIZE 9

#define COLOR_BACK_LIGHT 0x1e1e1e
#define COLOR_BACK_DARK 0x1c1c1c

#define COLOR_BACK_SELECT_STRING 0x333333

#define COLOR_BACK_SELECT_NUM 0x4aab2e
#define COLOR_BACK_NUM 0x592447

#define COLOR_OBJ_SELECT_NUM 0x5d5025
#define COLOR_OBJ_NUM 0x877fd7

#define BOX_PADDING_X 2
#define BOX_PADDING_Y 1

#define MAX_ITEM_COUNT 256
///////////////////////////////////////////

namespace JBF{
    namespace Debug{
        namespace Logger{
            // Structure(s) definition
            ///////////////////////////////////////////
            template<size_t N> class DLG_TEMPLATE{
            private:
                std::vector<BYTE> v;

            public:
                DLG_TEMPLATE() : v(N){
                    v.clear();
                }

            public:
                INLINE LPCDLGTEMPLATE Template(){ return (LPCDLGTEMPLATE)&v[0]; }
                void AlignToDword(){ if (v.size() % 4)Write(NULL, 4 - (v.size() % 4)); }
                void Write(LPCVOID pvWrite, DWORD cbWrite){
                    v.insert(v.end(), cbWrite, 0);
                    if (pvWrite)CopyMemory(&v[v.size() - cbWrite], pvWrite, cbWrite);
                }
                template<typename T> INLINE void Write(T t) { Write(&t, sizeof(T)); }
                INLINE void WriteString(LPCTSTR psz){ Write(psz, (lstrlenW(psz) + 1) * sizeof(TCHAR)); }
            };

            struct LIST_ITEM{
                TCHAR num[8];
                TCHAR str[1024];
                LOGGER_COLOR attribute;
                WORD hig;
            };
            ///////////////////////////////////////////

            // Inner variable(s) definition
            ///////////////////////////////////////////
            static std::mutex ins_pushMutex;

            static std::list<LIST_ITEM> ins_stringTable;
            static size_t ins_totalCount = 0;

            static HFONT ins_defaultFont = nullptr;

            static HWND ins_listStringHwnd = nullptr;
            static HWND ins_listNumHwnd = nullptr;

            static HWND ins_loggerHwnd = nullptr;
            ///////////////////////////////////////////

            // Inner function(s) definition
            ///////////////////////////////////////////
            static INLINE decltype(ins_stringTable)::iterator ins_getIteratorByIndex(size_t idx){
                auto i = ins_stringTable.begin();
                std::advance(i, idx);
                return i;
            }
            ///////////////////////////////////////////

            static INT_PTR CALLBACK ins_dlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
                static HBRUSH backColorLight = nullptr;
                static HBRUSH backColorDark = nullptr;

                switch (msg){
                case WM_INITDIALOG:
                {
                    ins_loggerHwnd = hDlg;
                    ins_listStringHwnd = GetDlgItem(ins_loggerHwnd, IDC_LISTSTR);
                    ins_listNumHwnd = GetDlgItem(ins_loggerHwnd, IDC_LISTNUM);

                    {
                        LOGFONT fntLog = { 0 };
                        tstrcpy_s(fntLog.lfFaceName, _countof(fntLog.lfFaceName), FONT_NAME);

                        auto dc = GetDC(ins_listStringHwnd);
                        fntLog.lfHeight = -MulDiv(FONT_SIZE, GetDeviceCaps(dc, LOGPIXELSY), 72);
                        ReleaseDC(ins_listStringHwnd, dc);

                        ins_defaultFont = CreateFontIndirect(&fntLog);
                    }

                    {
                        backColorLight = CreateSolidBrush(COLOR_BACK_LIGHT);
                        backColorDark = CreateSolidBrush(COLOR_BACK_DARK);
                    }

                    break;
                }
                case WM_DESTROY:
                {
                    DeleteObject(ins_defaultFont);

                    DeleteObject(backColorLight);
                    DeleteObject(backColorDark);
                    break;
                }

                case WM_CTLCOLORLISTBOX:
                {
                    auto hWnd = (HWND)lParam;

                    if (hWnd == ins_listStringHwnd){
                        return (LRESULT)(ins_stringTable.size() % 2 ? backColorLight : backColorDark);
                    }
                    else if (hWnd == ins_listNumHwnd){
                        return (LRESULT)(ins_stringTable.size() % 2 ? backColorDark : backColorLight);
                    }
                    break;
                }

                case WM_MEASUREITEM:
                {
                    auto mur = (LPMEASUREITEMSTRUCT)lParam;

                    if (mur->CtlType == ODT_LISTBOX){
                        switch (mur->CtlID){
                        case IDC_LISTSTR:
                        case IDC_LISTNUM:
                            mur->itemHeight = ins_getIteratorByIndex(mur->itemID)->hig;
                            break;
                        }
                    }
                    break;
                }
                case WM_DRAWITEM:
                {
                    auto lDraw = (LPDRAWITEMSTRUCT)lParam;

                    if (lDraw->CtlType == ODT_LISTBOX && lDraw->itemID != -1){
                        auto dc = lDraw->hDC;

                        auto oldTexCol = GetTextColor(dc);
                        auto oldBkCol = GetBkColor(dc);

                        auto fntOld = (HFONT)SelectObject(dc, ins_defaultFont);

                        if (lDraw->CtlID == IDC_LISTSTR){
                            auto itr = ins_getIteratorByIndex(lDraw->itemID);

                            SetTextColor(dc, (COLORREF)itr->attribute);
                            if ((lDraw->itemAction | ODA_SELECT) && (lDraw->itemState & ODS_SELECTED))SetBkColor(dc, COLOR_BACK_SELECT_STRING);
                            else if (lDraw->itemID % 2)SetBkColor(dc, COLOR_BACK_LIGHT);
                            else SetBkColor(dc, COLOR_BACK_DARK);

                            auto hbr = lDraw->itemID % 2 ? CreateSolidBrush(COLOR_BACK_LIGHT) : CreateSolidBrush(COLOR_BACK_DARK);
                            FillRect(dc, &lDraw->rcItem, hbr);
                            DeleteObject(hbr);

                            lDraw->rcItem.left += BOX_PADDING_X; lDraw->rcItem.top += BOX_PADDING_Y;
                            lDraw->rcItem.right -= BOX_PADDING_X; lDraw->rcItem.bottom -= BOX_PADDING_Y;
                            DrawTextEx(
                                dc, itr->str, (int)tstrlen(itr->str), &lDraw->rcItem,
                                DT_LEFT | DT_NOPREFIX | DT_TOP | DT_WORDBREAK, nullptr
                                );
                            lDraw->rcItem.left -= BOX_PADDING_X; lDraw->rcItem.top -= BOX_PADDING_Y;
                            lDraw->rcItem.right += BOX_PADDING_X; lDraw->rcItem.bottom += BOX_PADDING_Y;
                        }
                        else if (lDraw->CtlID == IDC_LISTNUM){
                            auto itr = ins_getIteratorByIndex(lDraw->itemID);

                            static HBRUSH hbr = nullptr;
                            if ((lDraw->itemAction | ODA_SELECT) && (lDraw->itemState & ODS_SELECTED)){
                                SetTextColor(dc, COLOR_OBJ_SELECT_NUM);
                                SetBkColor(dc, COLOR_BACK_SELECT_NUM);
                                hbr = CreateSolidBrush(COLOR_BACK_SELECT_NUM);
                            }
                            else{
                                SetTextColor(dc, COLOR_OBJ_NUM);
                                SetBkColor(dc, COLOR_BACK_NUM);
                                hbr = CreateSolidBrush(COLOR_BACK_NUM);
                            }
                            FillRect(dc, &lDraw->rcItem, hbr);
                            DeleteObject(hbr);

                            lDraw->rcItem.left += BOX_PADDING_X; lDraw->rcItem.top += BOX_PADDING_Y;
                            lDraw->rcItem.right -= BOX_PADDING_X; lDraw->rcItem.bottom -= BOX_PADDING_Y;
                            DrawTextEx(
                                dc, itr->num, (int)tstrlen(itr->num), &lDraw->rcItem,
                                DT_RIGHT | DT_NOPREFIX | DT_TOP | DT_WORDBREAK, nullptr
                                );
                            lDraw->rcItem.left -= BOX_PADDING_X; lDraw->rcItem.top -= BOX_PADDING_Y;
                            lDraw->rcItem.right += BOX_PADDING_X; lDraw->rcItem.bottom += BOX_PADDING_Y;
                        }

                        SelectObject(dc, fntOld);

                        SetBkColor(dc, oldBkCol);
                        SetTextColor(dc, oldTexCol);
                    }
                    break;
                }
                break;

                case WM_COMMAND:
                {
                    auto _event = HIWORD(wParam);
                    auto _id = LOWORD(wParam);

                    switch (_event){
                    case LBN_SELCHANGE:
                    case LBN_SETFOCUS:
                    {
                        LRESULT index = -1;
                        auto hWnd = (HWND)lParam;

                        if (_id == IDC_LISTSTR){
                            index = SNDMSG(hWnd, LB_GETCURSEL, 0, 0);
                            SNDMSG(GetDlgItem(hDlg, IDC_LISTNUM), LB_SETCURSEL, index, 0);
                        }
                        else if (_id == IDC_LISTNUM){
                            index = SNDMSG(hWnd, LB_GETCURSEL, 0, 0);
                            SNDMSG(GetDlgItem(hDlg, IDC_LISTSTR), LB_SETCURSEL, index, 0);
                        }
                        break;
                    }
                    }
                    break;
                }

                default:
                    return FALSE;
                }

                return TRUE;
            }

            INLINE HWND GetHandle(){ return ins_loggerHwnd; }

            void Create(HINSTANCE hInst){
                ins_stringTable.resize(MAX_ITEM_COUNT);
                ins_stringTable.clear();

                InitCommonControls();

                bool res = false;
                HDC hdc = GetDC(nullptr);
                if (hdc){
                    NONCLIENTMETRICSW ncm = { sizeof(ncm) };
                    if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0)){
                        DLG_TEMPLATE<128> tmp;

                        { // Dialog frame
                            // extended dialog header
                            tmp.Write<WORD>(1); // dialog version
                            tmp.Write<WORD>(0xffff); // extended dialog template
                            tmp.Write<DWORD>(0); // help ID
                            tmp.Write<DWORD>(WS_EX_NOACTIVATE);
                            tmp.Write<DWORD>(DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION);
                            tmp.Write<WORD>(2); // number of controls
                            tmp.Write<WORD>(0); // x
                            tmp.Write<WORD>(0); // y
                            tmp.Write<WORD>(SIZE_LISTSTR_X + SIZE_LISTNUM_X); // width
                            tmp.Write<WORD>(SIZE_LIST_Y); // height
                            tmp.WriteString(_T("")); // no menu
                            tmp.WriteString(_T("")); // default dialog class
                            tmp.WriteString(_T("Logger")); // title

                            // font description
                            if (ncm.lfMessageFont.lfHeight < 0)ncm.lfMessageFont.lfHeight = -MulDiv(
                                ncm.lfMessageFont.lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY)
                                );
                            tmp.Write<WORD>((WORD)ncm.lfMessageFont.lfHeight); // point
                            tmp.Write<WORD>((WORD)ncm.lfMessageFont.lfWeight); // weight
                            tmp.Write<BYTE>(ncm.lfMessageFont.lfItalic); // Italic
                            tmp.Write<BYTE>(ncm.lfMessageFont.lfCharSet); // CharSet
                            tmp.WriteString(ncm.lfMessageFont.lfFaceName);
                        }

                        { // IDC_LISTSTR
                            tmp.AlignToDword();
                            tmp.Write<DWORD>(0); // help id
                            tmp.Write<DWORD>(WS_EX_RTLREADING); // window extended style
                            tmp.Write<DWORD>(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT); // style
                            tmp.Write<WORD>(SIZE_LISTNUM_X); // x
                            tmp.Write<WORD>(0); // y
                            tmp.Write<WORD>(SIZE_LISTSTR_X); // width
                            tmp.Write<WORD>(SIZE_LIST_Y); // height
                            tmp.Write<DWORD>(IDC_LISTSTR); // control ID
                            tmp.Write<DWORD>(0x0083ffff); // control type
                            tmp.WriteString(_T("")); // text
                            tmp.Write<WORD>(0); // extra data
                        }

                        { // IDC_LISTNUM
                            tmp.AlignToDword();
                            tmp.Write<DWORD>(0); // help id
                            tmp.Write<DWORD>(WS_EX_RIGHT | WS_EX_RTLREADING); // window extended style
                            tmp.Write<DWORD>(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT); // style
                            tmp.Write<WORD>(0); // x
                            tmp.Write<WORD>(0); // y
                            tmp.Write<WORD>(SIZE_LISTNUM_X); // width
                            tmp.Write<WORD>(SIZE_LIST_Y); // height
                            tmp.Write<DWORD>(IDC_LISTNUM); // control ID
                            tmp.Write<DWORD>(0x0083ffff); // control type
                            tmp.WriteString(_T("")); // text
                            tmp.Write<WORD>(0); // extra data
                        }

                        res = CreateDialogIndirect(hInst, tmp.Template(), nullptr, ins_dlgProc) >= 0;
                    }
                }
                ReleaseDC(nullptr, hdc);
                PRIMITIVE_ASSERT(res, _T("Failed to create DebugLogger."));

                ShowWindow(ins_loggerHwnd, SW_SHOW);
            }
            void Release(){
                DestroyWindow(ins_loggerHwnd);
                ins_loggerHwnd = nullptr;
                ins_listStringHwnd = nullptr;
                ins_listNumHwnd = nullptr;
            }

            void ins_push(LIST_ITEM& add){
                std::lock_guard<decltype(ins_pushMutex)> scopedLock(ins_pushMutex);

                auto pos = ins_stringTable.size();
                if (pos >= MAX_ITEM_COUNT){
                    SNDMSG(ins_listNumHwnd, LB_DELETESTRING, 0, 0);
                    SNDMSG(ins_listStringHwnd, LB_DELETESTRING, 0, 0);
                    ins_stringTable.pop_front();
                    --pos;
                }

                {
                    RECT listRT;
                    GetClientRect(ins_listStringHwnd, &listRT);

                    auto dc = GetDC(ins_listStringHwnd);

                    auto fntOld = (HFONT)SelectObject(dc, ins_defaultFont);
                    RECT rt = { 0, 0, listRT.right - listRT.left, 0 };
                    rt.left += BOX_PADDING_X; rt.top += BOX_PADDING_Y;
                    rt.right -= BOX_PADDING_X; rt.bottom -= BOX_PADDING_Y;
                    DrawTextEx(
                        dc, add.str, (int)(tstrlen(add.str)), &rt,
                        DT_CALCRECT | DT_LEFT | DT_NOPREFIX | DT_TOP | DT_WORDBREAK, nullptr
                    );
                    rt.left -= BOX_PADDING_X; rt.top -= BOX_PADDING_Y;
                    rt.right += BOX_PADDING_X; rt.bottom += BOX_PADDING_Y;
                    SelectObject(dc, fntOld);

                    ReleaseDC(ins_listStringHwnd, dc);

                    add.hig = decltype(add.hig)(rt.bottom);
                }

                ins_stringTable.emplace_back(add);
                SNDMSG(ins_listNumHwnd, LB_ADDSTRING, 0, (LPARAM)ins_stringTable.back().num);
                SNDMSG(ins_listStringHwnd, LB_ADDSTRING, 0, (LPARAM)ins_stringTable.back().str);

                SNDMSG(ins_listNumHwnd, LB_SETCURSEL, pos, 0);
                SNDMSG(ins_listStringHwnd, LB_SETCURSEL, pos, 0);
            }
            void Push(LOGGER_COLOR attribute, LPCSTR str, ...){
                va_list iterat;
                LIST_ITEM add;

                add.attribute = attribute;

#ifdef UNICODE
#ifdef _WIN64
                _ui64tow_s(++ins_totalCount, add.num, _countof(add.num), 10);
#else
                _ultow_s(++ins_totalCount, add.num, _countof(add.num), 10);
#endif

                size_t orgLen = strlen(str);
                size_t convLen = MultiByteToWideChar(CP_ACP, 0, str, orgLen, nullptr, NULL);
                std::wstring tmpStr(convLen, 0);
                MultiByteToWideChar(CP_ACP, 0, str, orgLen, &tmpStr[0], convLen);

                va_start(iterat, tmpStr);
                vswprintf_s(add.str, tmpStr.c_str(), iterat);
                va_end(iterat);
#else
#ifdef _WIN64
                _ui64toa_s(++ins_totalCount, add.num, _countof(add.num), 10);
#else
                _ultoa_s(++ins_totalCount, add.num, _countof(add.num), 10);
#endif

                va_start(iterat, str);
                vsprintf_s(add.str, str, iterat);
                va_end(iterat);
#endif

                add.str[_countof(add.str) - 1] = 0;
                ins_push(add);
            }
            void Push(LOGGER_COLOR attribute, LPCWSTR str, ...){
                va_list iterat;
                LIST_ITEM add;

                add.attribute = attribute;

#ifdef UNICODE
#ifdef _WIN64
                _ui64tow_s(++ins_totalCount, add.num, _countof(add.num), 10);
#else
                _ultow_s(++ins_totalCount, add.num, _countof(add.num), 10);
#endif

                va_start(iterat, str);
                vswprintf_s(add.str, str, iterat);
                va_end(iterat);
#else
#ifdef _WIN64
                _ui64toa_s(++ins_totalCount, add.num, _countof(add.num), 10);
#else
                _ultoa_s(++ins_totalCount, add.num, _countof(add.num), 10);
#endif

                size_t orgLen = wcslen(str);
                size_t convLen = WideCharToMultiByte(CP_UTF8, 0, str, orgLen, nullptr, 0, nullptr, nullptr);
                std::string tmpStr(convLen, 0);
                WideCharToMultiByte(CP_UTF8, 0, str, orgLen, &tmpStr[0], convLen, nullptr, nullptr);

                va_start(iterat, tmpStr);
                vsprintf_s(add.str, tmpStr.c_str(), iterat);
                va_end(iterat);
#endif

                add.str[_countof(add.str) - 1] = 0;
                ins_push(add);
            }
        };
    };
};
#endif