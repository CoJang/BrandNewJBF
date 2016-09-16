#pragma once

#include<iostream>
#include<mutex>
#include<vector>
#include<unordered_map>

#include"JBF/Definitions.h"
#include"JBF/Global/Hash.h"

namespace JBF{
    namespace Global{
        namespace Archive{
            extern INLINE void MakeXORValue(void* src, size_t len, byte xor);

            extern void ObfuscateData(void* src, size_t len);

            class Decrypter{
            public:
                struct Data{
                    QWORD pos;
                    DWORD size;
                };

            private:
                static std::vector<byte> ins_rawBuffer;
                static std::mutex ins_readMutex;

            private:
                TCHAR ins_filePath[1024];
                FILE* ins_file;

            private:
                std::unordered_map<ARCHIVE_HASHSIZE, Data, Hash::HashedKeyHasher<ARCHIVE_HASHSIZE>> ins_dataTable;

            public:
                Decrypter();
                virtual ~Decrypter();

            public:
                bool OpenFile(const TCHAR* filePath);
                void CloseFile();

            public:
                bool GetDataLock(ARCHIVE_HASHSIZE key, void** buffer, DWORD* size);
                INLINE void Unlock(){ ins_readMutex.unlock(); }

            public:
                INLINE const TCHAR* GetFilePath()const{ return ins_filePath; }

            private:
                bool ins_loadInfo();
            };
        };
    };
};