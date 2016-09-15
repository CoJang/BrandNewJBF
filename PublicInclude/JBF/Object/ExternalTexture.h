#pragma once

#include<unordered_map>

#include"JBF/Definitions.h"
#include"JBF/Base/Base.h"
#include"JBF/Global/Archive.h"

namespace JBF{
    namespace Object{
        class ExternalTexture : public Base::Texture{
        private:
            struct DATA_KEY{
                Global::Archive::Decrypter* arc;
                ARCHIVE_HASHSIZE file;

                DATA_KEY& operator=(const DATA_KEY& rhs);
                bool operator==(const DATA_KEY& rhs)const;
            };
            struct DATA_HASHER{
                size_t operator()(const DATA_KEY& key)const;
            };

        private:
            static std::unordered_map<DATA_KEY, std::pair<ExternalTexture*, DWORD>, DATA_HASHER> ins_table;

        private:
            DATA_KEY ins_file;

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