#pragma once

#include<string>
#include<forward_list>
#include<vector>
#include<unordered_map>

#include<tchar.h>
#include<windows.h>
#include"JBF/Definitions.h"
#include"JBF/Global/Hash.h"
#include"JBF/Global/Archive.h"

struct RAW_FILE{
    std::basic_string<TCHAR> filePath;
    std::basic_string<TCHAR> fileLocalPath;
    FILETIME fileTime;
    DWORD fileSize;
};
struct ARCHIVE_FORMAT{
    bool isCorrectedCache;
    std::basic_string<TCHAR> archivePath;
    std::basic_string<TCHAR> cachePath;
    std::unordered_map<ARCHIVE_HASHSIZE, RAW_FILE, JBF::Global::Hash::HashedKeyHasher<ARCHIVE_HASHSIZE>> fileList;
};

extern std::forward_list<ARCHIVE_FORMAT*> ArchiveList;

extern bool SearchDirectory(const std::basic_string<TCHAR>& filePath);

extern bool CacheCorrectionCheck(const ARCHIVE_FORMAT* archive);
extern bool MakeCache(const ARCHIVE_FORMAT* archive);

extern bool MakeArchive(const std::basic_string<TCHAR>& outputPath, ARCHIVE_FORMAT* archive, byte xor);