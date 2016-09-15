#pragma once

#include<unordered_map>

#include"JBF/Definitions.h"
#include"JBF/Base/Base.h"
#include"JBF/Global/Global.h"

namespace JBF{
    namespace Object{
        class ExternalTexture : public Base::Texture{
        private:
            static std::unordered_map<size_t, std::pair<ExternalTexture*, DWORD>, Global::Hash::HashedKeyHasher<size_t>> ins_table;
            static INLINE size_t ins_makeTableKey(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file){
                return reinterpret_cast<size_t>(arc) ^ file;
            }

        private:
            Global::Archive::Decrypter* ins_archive;
            ARCHIVE_HASHSIZE ins_file;

        public:
            static void InitTable();
            static void ReleaseTable();

        private:
            ExternalTexture();
            virtual ~ExternalTexture();

        public:
            static ExternalTexture* Read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file);
            static void Release(ExternalTexture* obj);

        public:
            virtual HRESULT Validate();
            virtual void Invalidate();
        };
    };
};