#pragma once

namespace JBF{
    namespace Base{
        class DXResource{
        public:
            enum RES_TYPE : BYTE{ RESTYPE_MANAGE, RESTYPE_VRAM };

        private:
            const RES_TYPE ins_resType;

        public:
            DXResource(RES_TYPE type);
            virtual ~DXResource();

        public:
            virtual HRESULT Validate();
            virtual void Invalidate();

        public:
            INLINE const RES_TYPE GetResourceType()const{ return ins_resType; }
        };
    };
};