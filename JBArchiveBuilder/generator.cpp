#include"main.h"

using namespace JBF::Global;

// Archive struct
// -File count
// --File hash
// --File size
// --File data

// Inner function(s) definition
///////////////////////////////////////////
void ins_getFileSize(ARCHIVE_FORMAT* archive){
    FILE* file;

    for (auto i = archive->fileList.begin(), e = archive->fileList.end(); i != e; ++i){
        _tfopen_s(&file, i->second.filePath.c_str(), _T("rb"));
        if (!file)continue;

        if (fseek(file, 0, SEEK_END))continue;
        i->second.fileSize = ftell(file);
        if (fseek(file, 0, SEEK_SET))continue;

        fclose(file);
    }
}
bool ins_readFile(const std::basic_string<TCHAR>& filePath, byte* dest, size_t len){
    FILE* file;

    _tfopen_s(&file, filePath.c_str(), _T("rb"));
    if (!file)goto FAILED_FUNC;

    if (fread(dest, sizeof(decltype(*dest)), len, file) != len)goto FAILED_FUNC;

    fclose(file);
    return true;

FAILED_FUNC:
    if (file)fclose(file);
    return false;
}

bool ins_makeArchive(const std::basic_string<TCHAR>& outputPath, ARCHIVE_FORMAT* archive, byte xor){
    FILE* file = nullptr;
    DWORD tmp;
    std::vector<byte> buffer(50 * 1024 * 1024);

    buffer.clear();

    _tfopen_s(&file, (outputPath + _T("\\") + archive->archivePath).c_str(), _T("wb"));
    if (!file)goto FAILED_FUNC;

    tmp = archive->fileList.size();
    Archive::MakeXORValue(&tmp, sizeof(decltype(tmp)), xor);
    if (fwrite(&tmp, sizeof(decltype(tmp)), 1, file) != 1)goto FAILED_FUNC;

    for (auto i : archive->fileList){
        {
            tmp = i.first;
            Archive::MakeXORValue(&tmp, sizeof(decltype(tmp)), xor);
            if (fwrite(&tmp, sizeof(decltype(tmp)), 1, file) != 1)goto FAILED_FUNC;

            tmp = i.second.fileSize;
            Archive::MakeXORValue(&tmp, sizeof(decltype(tmp)), xor);
            if (fwrite(&tmp, sizeof(decltype(tmp)), 1, file) != 1)goto FAILED_FUNC;
        }

        {
            buffer.resize(i.second.fileSize);
            if (!ins_readFile(i.second.filePath, &buffer[0], i.second.fileSize))goto FAILED_FUNC;
        }

        {
            Archive::ObfuscateData(&buffer[0], i.second.fileSize);
            Archive::MakeXORValue(&buffer[0], i.second.fileSize, xor);
            if (fwrite(&buffer[0], sizeof(decltype(buffer[0])), i.second.fileSize, file) != i.second.fileSize)goto FAILED_FUNC;
        }
    }

    fclose(file);
    return true;

FAILED_FUNC:
    if (file)fclose(file);
    return false;
}
///////////////////////////////////////////

bool MakeArchive(const std::basic_string<TCHAR>& outputPath, ARCHIVE_FORMAT* archive, byte xor){
    ins_getFileSize(archive);
    if (!ins_makeArchive(outputPath, archive, xor))return false;
    return true;
}