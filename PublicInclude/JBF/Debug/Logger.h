#ifdef _DEBUG
#pragma once

#include<tchar.h>
#include"JBF/Definitions.h"
#include"JBF/Global/Global.h"

namespace JBF{
    namespace Debug{
        namespace Logger{
            enum LOGGER_COLOR : COLORREF{
                LC_NORMAL = 0xdadada,
                LC_INFO = 0x38c235,
                LC_SUCCEEDED = 0xc27d35,
                LC_WARNED = 0x35c0c2,
                LC_FAILED = 0x3535c2
            };

            INLINE extern HWND GetHandle();

            extern void Create(HINSTANCE hInst);
            extern void Release();
            extern void Push(LOGGER_COLOR attribute, LPCSTR str, ...);
            extern void Push(LOGGER_COLOR attribute, LPCWSTR str, ...);
        };
    };
};

#define __W(x) L##x
#define __A(x) x
#define _W(x) __W(x)
#define _A(x) __A(x)

#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)

#define ASSERTA(isFalse, str, ...) \
    if(!(bool)(isFalse)){ \
        if(JBF::Debug::Logger::GetHandle()){ \
            JBF::Debug::Logger::Push( \
                JBF::Debug::Logger::LC_FAILED, \
                "File: " _A(__FILE__) "\n" "Line: " STRINGIFY_BUILTIN(__LINE__) \
                ); \
        } \
        JBF::Global::MsgBox( \
            nullptr, \
            "Assertion failed", \
            MB_ICONERROR | MB_OK, \
            "Assertion failed!\n\n" "File: " _A(__FILE__) "\n" "Line: " STRINGIFY_BUILTIN(__LINE__) "\n\n" "Expression: " STRINGIFY_BUILTIN(isFalse) "\n\n" str \
            __VA_ARGS__ \
            ); \
        __debugbreak(); \
    }
#define ASSERTW(isFalse, str, ...) \
    if(!(bool)(isFalse)){ \
        if(JBF::Debug::Logger::GetHandle()){ \
            JBF::Debug::Logger::Push( \
                JBF::Debug::Logger::LC_FAILED, \
                L"File: " _W(__FILE__) L"\n" L"Line: " STRINGIFY_BUILTIN(__LINE__) \
                ); \
        } \
        JBF::Global::MsgBox( \
            nullptr, \
            L"Assertion failed", \
            MB_ICONERROR | MB_OK, \
            L"Assertion failed!\n\n" L"File: " _W(__FILE__) L"\n" L"Line: " STRINGIFY_BUILTIN(__LINE__) L"\n\n" L"Expression: " STRINGIFY_BUILTIN(isFalse) L"\n\n" str, \
            __VA_ARGS__ \
            ); \
        __debugbreak(); \
    }

#define ASSERT_HRESULTA(hr, str, ...) \
    if(FAILED(hr)){ \
        if(JBF::Debug::Logger::GetHandle()){ \
            JBF::Debug::Logger::Push( \
                JBF::Debug::Logger::LC_FAILED, \
                "File: " _A(__FILE__) "\n" "Line: " STRINGIFY_BUILTIN(__LINE__) "\n" "Value: %X", \
                hr \
                ); \
        } \
        JBF::Global::MsgBox( \
            nullptr, \
            "HRESULT failed", \
            MB_ICONERROR | MB_OK, \
            "HRESULT failed!\n\n" "File: " _A(__FILE__) "\n" "Line: " STRINGIFY_BUILTIN(__LINE__) "\n\n" "Expression: " STRINGIFY_BUILTIN(hr) "\n\n" str, \
            __VA_ARGS__ \
            ); \
        __debugbreak(); \
    }
#define ASSERT_HRESULTW(hr, str, ...) \
    if(FAILED(hr)){ \
        if(JBF::Debug::Logger::GetHandle()){ \
            JBF::Debug::Logger::Push( \
                JBF::Debug::Logger::LC_FAILED, \
                L"File: " _W(__FILE__) L"\n" L"Line: " STRINGIFY_BUILTIN(__LINE__) L"\n" L"Value: %X", \
                hr \
                ); \
        } \
        JBF::Global::MsgBox( \
            nullptr, \
            L"HRESULT failed", \
            MB_ICONERROR | MB_OK, \
            L"HRESULT failed!\n\n" L"File: " _W(__FILE__) L"\n" L"Line: " STRINGIFY_BUILTIN(__LINE__) L"\n\n" L"Expression: " STRINGIFY_BUILTIN(hr) L"\n\n" str, \
            __VA_ARGS__ \
            ); \
        __debugbreak(); \
    }

#ifdef UNICODE
#define ASSERT(isFalse, str, ...) ASSERTW(isFalse, str, __VA_ARGS__)
#define ASSERT_HRESULT(hr, str, ...) ASSERT_HRESULTW(hr, str, __VA_ARGS__)
#else
#define ASSERT(isFalse, str, ...) ASSERTA(isFalse, str, __VA_ARGS__)
#define ASSERT_HRESULT(hr, str, ...) ASSERT_HRESULTA(hr, str, __VA_ARGS__)
#endif

#define LOGGING(attribute, str, ...) \
    if(JBF::Debug::Logger::GetHandle()){ \
        JBF::Debug::Logger::Push( \
            attribute, \
            str, \
            __VA_ARGS__ \
        ); \
    }

#define LOGGING_NORMAL(str, ...) LOGGING(JBF::Debug::Logger::LC_NORMAL, str, __VA_ARGS__)
#define LOGGING_INFO(str, ...) LOGGING(JBF::Debug::Logger::LC_INFO, str, __VA_ARGS__)
#define LOGGING_SUCCEEDED(str, ...) LOGGING(JBF::Debug::Logger::LC_SUCCEEDED, str, __VA_ARGS__)
#define LOGGING_WARNED(str, ...) LOGGING(JBF::Debug::Logger::LC_WARNED, str, __VA_ARGS__)
#define LOGGING_FAILED(str, ...) LOGGING(JBF::Debug::Logger::LC_FAILED, str, __VA_ARGS__)
#else
#define ASSERT(isTrue, ...) (void)(isTrue)
#define ASSERT_HRESULT(hr, ...) (void)(hr)
#define LOGGING(attribute, ...) (void)(attribute)

#define LOGGING_NORMAL(...)
#define LOGGING_INFO(...)
#define LOGGING_SUCCEEDED(...)
#define LOGGING_WARNED(...)
#define LOGGING_FAILED(...)
#endif