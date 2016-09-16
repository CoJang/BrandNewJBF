#pragma once

#include<unordered_map>

#include"JBF/Definitions.h"
#include"JBF/Base/Base.h"
#include"JBF/Global/Archive.h"

namespace JBF{
    namespace Object{
        class ExternalTexture : public Base::Texture, public Base::ArchiveResource<ExternalTexture>{
        public:
            static void InitTable();
            static void ReleaseTable();

        private:
            ExternalTexture();
            virtual ~ExternalTexture();

        public:
            static ExternalTexture* Read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file);

        public:
            virtual HRESULT Validate();
            virtual void Invalidate();

        public:
            friend class Base::ArchiveResource<ExternalTexture>;
        };
    };
};