#include<Shlwapi.h>
#include"main.h"

#pragma comment(lib, "Shlwapi.lib")

std::forward_list<ARCHIVE_FORMAT*> ArchiveList;

BOOL ins_createDirectory(LPCTSTR lpszPath){
    TCHAR szPathBuffer[4096];

    size_t len = _tcslen(lpszPath);

    for (size_t i = 0; i < len; i++){
        szPathBuffer[i] = lpszPath[i];
        if (szPathBuffer[i] == _T('\\') || szPathBuffer[i] == _T('/')){
            szPathBuffer[i + 1] = 0;
            if (!PathFileExists(szPathBuffer)){
                if (!CreateDirectory(szPathBuffer, nullptr)){
                    if (GetLastError() != ERROR_ALREADY_EXISTS)return FALSE;
                }
            }
        }
    }
    return TRUE;
}

void release(){
    while (!ArchiveList.empty()){
        auto obj = *ArchiveList.begin();
        delete obj;
        ArchiveList.pop_front();
    }
}
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]){
    std::basic_string<TCHAR> contentPath = _T("..\\RawContent");
    std::basic_string<TCHAR> archivePath = _T(".\\Content");

    ins_createDirectory((archivePath + _T('\\')).c_str());

    if (!SearchDirectory(contentPath)){
        tprintf_s(_T("Failed to search directory \"%s\".\n"), contentPath.c_str());
        goto END_FUNC;
    }
    for (auto i = ArchiveList.begin(), e = ArchiveList.end(); i != e; ++i){
        auto cur = *i;

        if (cur->fileList.empty())continue;

        if (CacheCorrectionCheck(cur))continue;

        if (!MakeArchive(archivePath, cur, ARCHIVE_XORBYTE))tprintf_s(_T("Failed to make \"%s\".\n"), cur->cachePath.c_str());

        if (!MakeCache(cur))tprintf_s(_T("Failed to make cache \"%s\".\n"), cur->cachePath.c_str());
    }

END_FUNC:
    return 0;
}