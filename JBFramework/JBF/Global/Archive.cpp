#include"pch.h"

#include<ppl.h>

#include"JBF/Debug/Debug.h"
#include"JBF/Global/Global.h"

// Archive struct
// -File count
// --File hash
// --File size
// --File data

namespace JBF{
    namespace Global{
        namespace Archive{
            INLINE void MakeXORValue(void* src, size_t len, byte xor){
                byte shiftSize = xor / 2;
                byte* p = (decltype(p))src;

                if (!shiftSize)shiftSize = 97;
                for (auto i = p; (i - p) < len; ++i){
                    *i ^= xor;
                    xor = Math::BitRotateLeft(xor, shiftSize);
                }
            }

            void ObfuscateData(void* src, size_t len){
                byte* pSrc = (decltype(pSrc))src;
                size_t sideCount;
                size_t posMid;

                do{
                    sideCount = sqrt((float)len);
                    posMid = sideCount / 2;

                    Concurrency::parallel_for<DWORD>(0u, sideCount, [=](DWORD i){
                        decltype(pSrc) cur = pSrc + i;
                        for (DWORD n = 0; n < posMid; ++n)std::swap(cur[n], cur[sideCount - n - 1]);
                    });

                    len -= sideCount * sideCount;
                } while (sideCount > 1);
            }


            std::vector<byte> Decrypter::ins_rawBuffer(10 << 20);
            std::mutex Decrypter::ins_readMutex;

            Decrypter::Decrypter(){
                ins_file = nullptr;
            }
            Decrypter::~Decrypter(){
                CloseFile();
            }

            bool Decrypter::OpenFile(const TCHAR* filePath){
                if (ins_file)CloseFile();

                tstrcpy_s(ins_filePath, filePath);

                _tfopen_s(&ins_file, ins_filePath, _T("rb"));
                if (!ins_file)goto FAILED_FUNC;

                if (!ins_loadInfo())goto FAILED_FUNC;

                return true;

            FAILED_FUNC:
                CloseFile();
                LOGGING_FAILED(_T("Failed to load archive.\n\"%s\""), ins_filePath);
                return false;
            }
            void Decrypter::CloseFile(){
                if (ins_file){
                    fclose(ins_file);
                    ins_file = nullptr;
                }
            }

            bool Decrypter::ins_loadInfo(){
                DWORD count;

                if (_fseeki64(ins_file, 0, SEEK_SET))return false;

                if (fread(&count, sizeof(decltype(count)), 1, ins_file) != 1)return false;
                MakeXORValue(&count, sizeof count, ARCHIVE_XORBYTE);

                ins_dataTable.rehash(count + ((log2(count) * ARCHIVE_ARERAGE_FILE_COUNT) / (sizeof(ARCHIVE_HASHSIZE) * 8)));

                for (auto i = decltype(count){0}; i < count; ++i){
                    ARCHIVE_HASHSIZE key;
                    Data add;

                    if (fread(&key, sizeof(decltype(key)), 1, ins_file) != 1)return false;
                    MakeXORValue(&key, sizeof key, ARCHIVE_XORBYTE);

                    if (fread(&add.size, sizeof(decltype(add.size)), 1, ins_file) != 1)return false;
                    MakeXORValue(&add.size, sizeof add.size, ARCHIVE_XORBYTE);

                    add.pos = _ftelli64(ins_file);

                    ins_dataTable.emplace(key, add);
                    
                    if (_fseeki64(ins_file, add.size, SEEK_CUR))return false;
                }

                return true;
            }

            bool Decrypter::GetDataLock(ARCHIVE_HASHSIZE key, void** buffer, DWORD* size){
                decltype(&ins_rawBuffer[0]) buf;
                auto itr = ins_dataTable.find(key);
                if (itr == ins_dataTable.end())return false;

                ins_readMutex.lock();
                {
                    ins_rawBuffer.clear();
                    ins_rawBuffer.resize(itr->second.size);
                    buf = &ins_rawBuffer[0];

                    if (_fseeki64(ins_file, itr->second.pos, SEEK_SET))return false;
                    if (fread(buf, sizeof(decltype(*buf)), itr->second.size, ins_file) != itr->second.size)return false;
                    MakeXORValue(buf, itr->second.size, ARCHIVE_XORBYTE);
                    ObfuscateData(buf, itr->second.size);
                }

                *buffer = buf;
                if (size)*size = itr->second.size;

                return true;
            }
        };
    };
};