#pragma once

#include"JBF/Definitions.h"
#include"JBF/Base/Base.h"

namespace JBF{
    namespace Object{
        class ExternalTexture : public Base::Texture, public Base::ArchiveResource<ExternalTexture>{
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