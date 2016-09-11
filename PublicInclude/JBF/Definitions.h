#pragma once

#include<tchar.h>
#include<windows.h>

#ifdef USE_INTRINSIC
#define MATH_CALL
// __vectorcall로 호출 규약을 설정해 두었으나, 일부 함수들에서 링크 에러가 발생함.
// __thiscall로 인식되며 이로 인해 시그니처 불일치가 나타나는 것으로 보임.
#else
#define MATH_CALL
#endif

#define INLINE __forceinline

#define ARCHIVE_XORBYTE 71
#define ARCHIVE_ARERAGE_FILE_COUNT 4096
#define ARCHIVE_HASHSIZE unsigned long

typedef unsigned __int64 QWORD;

#define RELEASE(d) { d->Release(); d = nullptr; }
#define ARRAY_RELEASE(d, size) { for(size_t i = 0; i < size; ++i)RELEASE(d[i]); }

extern int (WINAPIV *__vsnprintf)(char*, size_t, const char*, va_list);
extern int (WINAPIV *__vsnwprintf)(wchar_t*, size_t, const wchar_t*, va_list);

#ifdef UNICODE
#define CommandLineToArgv CommandLineToArgvW

#define tstrlen wcslen
#define tstrcpy_s wcscpy_s
#define tstrncpy_s wcsncpy_s

#define tprintf_s wprintf_s
#define stprintf_s swprintf_s
#define vstprintf_s vswprintf_s

#define _ultot_s _ultow_s
#define _ui64tot_s _ui64tow_s

#define _tfopen_s _wfopen_s
#else
#define CommandLineToArgv CommandLineToArgvA

#define tstrlen strlen
#define tstrcpy_s strcpy_s
#define tstrncpy_s strncpy_s

#define tprintf_s printf_s
#define stprintf_s sprintf_s
#define vstprintf_s vsprintf_s

#define _ultot_s _ultoa_s
#define _ui64tot_s _ui64toa_s

#define _tfopen_s _fopen_s
#endif