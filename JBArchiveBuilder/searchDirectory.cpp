#include"main.h"

using namespace JBF::Global;

// Inner function(s) definition
///////////////////////////////////////////
static DWORD ins_collectFile(std::basic_string<TCHAR>& filePath, const std::basic_string<TCHAR>& rootPath, ARCHIVE_FORMAT* rootInfo){
    WIN32_FIND_DATA fdInfo;
    HANDLE hFile;
    std::basic_string<TCHAR> curFile;

    curFile = filePath + _T("\\*.*");
    hFile = FindFirstFile(curFile.c_str(), &fdInfo);
    do{
        if (hFile == INVALID_HANDLE_VALUE)continue;
        if (fdInfo.cFileName[0] == _T('.'))continue;
        if (fdInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)continue;

        curFile = filePath + _T("\\") + fdInfo.cFileName;

        if (fdInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            auto ret = ins_collectFile(curFile, rootPath, rootInfo);
            if (ret){
                FindClose(hFile);
                return ret;
            }
        }
        else{
            std::basic_string<TCHAR> localPath = curFile.substr(rootPath.length() + 1);
            auto key = Hash::X65599Generator<ARCHIVE_HASHSIZE>(localPath.c_str(), localPath.length());

            for (auto i : rootInfo->fileList){
                if (key == i.first){
                    tprintf_s(_T("Duplicated hash key detected.\n\"%s\" and \"%s\".\n"), localPath.c_str(), i.second.fileLocalPath.c_str());
                    return 0xffffffff;
                }
            }

            RAW_FILE add;
            add.fileLocalPath = localPath;
            add.filePath = curFile;
            add.fileTime = fdInfo.ftLastWriteTime;

            rootInfo->fileList.emplace(key, add);
        }

    } while (FindNextFile(hFile, &fdInfo) == TRUE);

    FindClose(hFile);
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES)return dwError;
    }
    return 0;
}
///////////////////////////////////////////

bool SearchDirectory(const std::basic_string<TCHAR>& filePath){
    WIN32_FIND_DATA fdInfo;
    HANDLE hFile;
    std::basic_string<TCHAR> curFile;
    std::basic_string<TCHAR> curFileExt;

    curFile = filePath + _T("\\*.*");
    hFile = FindFirstFile(curFile.c_str(), &fdInfo);
    do{
        if (hFile == INVALID_HANDLE_VALUE)continue;
        if (fdInfo.cFileName[0] == _T('.'))continue;
        if (fdInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)continue;

        if (fdInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            ARCHIVE_FORMAT* add = new ARCHIVE_FORMAT();

            curFile = filePath + _T("\\") + fdInfo.cFileName;

            add->archivePath = fdInfo.cFileName;
            add->archivePath += _T(".jba");
            ArchiveList.emplace_front(add);

            add->cachePath = curFile + _T(".jba_cache");
            if (ins_collectFile(curFile, curFile, add))return false;
        }
    } while (FindNextFile(hFile, &fdInfo) == TRUE);

    FindClose(hFile);
    return true;
}