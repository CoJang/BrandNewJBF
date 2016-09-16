#pragma once

#include"JBF/Definitions.h"
#include"JBF/Base/Base.h"

namespace JBF{
    namespace Object{
        class Shader : public Base::DXResource, public Base::ArchiveResource<Shader>{
        private:
            ID3DXEffect* ins_effect;

        private:
            Shader();
            virtual ~Shader();

        public:
            static Shader* Read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file);

        public:
            virtual HRESULT Validate();
            virtual void Invalidate();

        public:
            friend class Base::ArchiveResource<Shader>;
        };
    };
};