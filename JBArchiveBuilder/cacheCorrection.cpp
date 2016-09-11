#include"main.h"

// Cache file struct
// -File count(DWORD)
// --File hash
// --File path length
// --File path
// --File time

// Inner structure(s) definition
///////////////////////////////////////////
struct CACHE_FORMAT{
    std::basic_string<TCHAR> path;
    FILETIME time;
};
///////////////////////////////////////////

bool CacheCorrectionCheck(const ARCHIVE_FORMAT* archive){
    DWORD i;
    DWORD tmp;
    DWORD count;

    DWORD filePtr = 0;
    HANDLE file = CreateFile(
        archive->cachePath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    if (file == INVALID_HANDLE_VALUE)goto CREATE_REQUIRED;

    if (!ReadFile(file, &count, sizeof count, &filePtr, nullptr))goto CREATE_REQUIRED;
    if (count != archive->fileList.size())goto CREATE_REQUIRED;

    for (i = 0; i < count; ++i){
        DWORD key;
        CACHE_FORMAT cur;
        decltype(archive->fileList)::const_iterator tar;

        if (!ReadFile(file, &key, sizeof key, &filePtr, nullptr))goto CREATE_REQUIRED;
        tar = archive->fileList.find(key);
        if (tar == archive->fileList.cend())goto CREATE_REQUIRED;

        if (!ReadFile(file, &tmp, sizeof tmp, &filePtr, nullptr))goto CREATE_REQUIRED;
        cur.path.resize(tmp);
        if (!ReadFile(file, &cur.path[0], tmp * sizeof TCHAR, &filePtr, nullptr))goto CREATE_REQUIRED;
        if (tar->second.fileLocalPath != cur.path)goto CREATE_REQUIRED;

        if (!ReadFile(file, &cur.time, sizeof cur.time, &filePtr, nullptr))goto CREATE_REQUIRED;
        if (tar->second.fileTime.dwHighDateTime != cur.time.dwHighDateTime)goto CREATE_REQUIRED;
        if (tar->second.fileTime.dwLowDateTime != cur.time.dwLowDateTime)goto CREATE_REQUIRED;
    }

    CloseHandle(file);
    return true;

CREATE_REQUIRED:
    if (file && (file == INVALID_HANDLE_VALUE))CloseHandle(file);
    return false;
}
bool MakeCache(const ARCHIVE_FORMAT* archive){
    DWORD tmp;
    const TCHAR* str;
    DWORD count;

    DWORD filePtr = 0;
    HANDLE file = CreateFile(
        archive->cachePath.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    if (file == INVALID_HANDLE_VALUE)goto FAILED_FUNC;

    count = archive->fileList.size();
    if (!WriteFile(file, &count, sizeof count, &filePtr, nullptr))goto FAILED_FUNC;

    for (auto i = archive->fileList.cbegin(), e = archive->fileList.cend(); i != e; ++i){
        if (!WriteFile(file, &i->first, sizeof i->first, &filePtr, nullptr))goto FAILED_FUNC;

        tmp = i->second.fileLocalPath.length();
        if (!WriteFile(file, &tmp, sizeof tmp, &filePtr, nullptr))goto FAILED_FUNC;

        str = i->second.fileLocalPath.c_str();
        if (!WriteFile(file, str, tmp * sizeof TCHAR, &filePtr, nullptr))goto FAILED_FUNC;

        if (!WriteFile(file, &i->second.fileTime, sizeof i->second.fileTime, &filePtr, nullptr))goto FAILED_FUNC;
    }

    CloseHandle(file);
    return true;

FAILED_FUNC:
    if (file && (file == INVALID_HANDLE_VALUE))CloseHandle(file);
    return false;
}