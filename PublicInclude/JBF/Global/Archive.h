#pragma once

#include<iostream>
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
            public:
                typedef std::unordered_map<ARCHIVE_HASHSIZE, Data, Hash::HashedKeyHasher<ARCHIVE_HASHSIZE>> DATA_TABLE;

            private:
                TCHAR ins_filePath[1024];
                FILE* ins_file;

            private:
                DATA_TABLE ins_dataTable;

            public:
                Decrypter();
                virtual ~Decrypter();

            public:
                bool OpenFile(const TCHAR* filePath);
                void CloseFile();

            public:
                DATA_TABLE::const_iterator GetSize(ARCHIVE_HASHSIZE key, DWORD* size);
                bool GetData(const DATA_TABLE::const_iterator& itr, void* buffer);

            private:
                bool ins_loadInfo();
            };
        };
    };
};