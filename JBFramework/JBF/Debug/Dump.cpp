#include"pch.h"
#include"JBF/Debug/Debug.h"
#include<ctime>
#include<cstdio>
#include<DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

// Predefinition(s)
///////////////////////////////////////////
#define DUMP_FILENAME _T("\\JBFCrashYYYY-MM-DD_HH.MM.SS.dmp")
#define DUMP_FILENAME_LENGTH _countof(DUMP_FILENAME)
///////////////////////////////////////////

namespace JBF{
    namespace Debug{
        namespace Dump{
            // Inner variable(s) definition
            ///////////////////////////////////////////
            static LPTSTR ins_dumpLocation = nullptr;
            static size_t ins_dumpLocationLength = 0;
            ///////////////////////////////////////////

            // Inner function(s) definition
            ///////////////////////////////////////////
            LONG WINAPI ins_topLevelFilter(_EXCEPTION_POINTERS* exceptionInfo){
                LONG res = EXCEPTION_CONTINUE_SEARCH;
				time_t curT = time(nullptr);
				tm t = {};
				localtime_s(&t, &curT);

                stprintf_s(
                    &ins_dumpLocation[ins_dumpLocationLength - DUMP_FILENAME_LENGTH],
                    DUMP_FILENAME_LENGTH,
                    _T("\\JBFCrash%04u-%02u-%02u_%02u.%02u.%02u.dmp"),
                    t.tm_year + 1900,
                    t.tm_mon + 1,
                    t.tm_mday,
                    t.tm_hour,
                    t.tm_min,
                    t.tm_sec
                );

                HANDLE file = CreateFile(
                    ins_dumpLocation,
                    GENERIC_WRITE,
                    FILE_SHARE_WRITE,
                    NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr
                );
                if (file != INVALID_HANDLE_VALUE){
                    _MINIDUMP_EXCEPTION_INFORMATION inf;

                    inf.ThreadId = GetCurrentThreadId();
                    inf.ExceptionPointers = exceptionInfo;
                    inf.ClientPointers = NULL;

                    if (MiniDumpWriteDump(
                        GetCurrentProcess(),
                        GetCurrentProcessId(),
                        file,
                        MiniDumpNormal,
                        &inf,
                        NULL,
                        NULL
                    ))res = EXCEPTION_EXECUTE_HANDLER;

                    CloseHandle(file);
                }

                return res;
            }
            ///////////////////////////////////////////

            void Init(){
                auto rawStr = GetCommandLine();
                int num = 0;

                LPTSTR* argv;

                argv = CommandLineToArgv(rawStr, &num);
                num = tstrlen(argv[0]);
                for (num -= 1; num >= 0; --num){ if (argv[0][num] == _T('\\'))break; }

                ins_dumpLocationLength = num + DUMP_FILENAME_LENGTH;
                ins_dumpLocation = (decltype(ins_dumpLocation))malloc(ins_dumpLocationLength * sizeof(decltype(*ins_dumpLocation)));

                tstrncpy_s(ins_dumpLocation, ins_dumpLocationLength, argv[0], num);

                LocalFree(argv);
            }
            void Release(){
                free(ins_dumpLocation);
            }

            INLINE void MakeDump(){
                SetUnhandledExceptionFilter(ins_topLevelFilter);
				exit(EXIT_FAILURE);
            }
        };
    };
};