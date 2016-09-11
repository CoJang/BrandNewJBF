#include"main.h"

std::forward_list<ARCHIVE_FORMAT*> ArchiveList;

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

    if (!SearchDirectory(contentPath)){
        tprintf_s(_T("Failed to search directory \"%s\".\n"), contentPath.c_str());
        goto END_FUNC;
    }
    for (auto i = ArchiveList.begin(), e = ArchiveList.end(); i != e; ++i){
        auto cur = *i;

        if (cur->fileList.empty())continue;

        if (CacheCorrectionCheck(cur))continue;

        if (!MakeArchive(archivePath, cur, ARCHIVE_XORBYTE))tprintf_s(_T("Failed to make cache \"%s\".\n"), cur->cachePath.c_str());

        if (!MakeCache(cur))tprintf_s(_T("Failed to make cache \"%s\".\n"), cur->cachePath.c_str());
    }

END_FUNC:
    release();
    {
        std::basic_string<TCHAR> appName = _T("Game.exe");
        std::basic_string<TCHAR> curPath = argv[0];
        curPath = curPath.substr(0, curPath.rfind(_T('\\')));
        ShellExecute(
            nullptr,
            _T("open"),
            (curPath + _T('\\') + appName).c_str(),
            nullptr,
            nullptr,
            SW_SHOW
        );
    }
    return 0;
}