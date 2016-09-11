#include"pch.h"

#include<unordered_set>

#include"JBF/Core/Graphic/Graphic.h"
#include"JBF/Base/Base.h"

namespace JBF{
    namespace Core{
        namespace Graphic{
            extern std::unordered_set<Base::DXResource*, Global::Hash::PointerKeyHasher<Base::DXResource*>> ins_resTableManaged, ins_resTableVRAM;
        };
    };

    namespace Base{
        DXResource::DXResource(RES_TYPE type) : ins_resType(type){
            switch (ins_resType){
            case RESTYPE_MANAGE:
                Core::Graphic::ins_resTableManaged.emplace(this);
                break;
            case RESTYPE_VRAM:
                Core::Graphic::ins_resTableVRAM.emplace(this);
                break;
            }
        }
        DXResource::~DXResource(){
            switch (ins_resType){
            case RESTYPE_MANAGE:
                Core::Graphic::ins_resTableManaged.erase(this);
                break;
            case RESTYPE_VRAM:
                Core::Graphic::ins_resTableVRAM.erase(this);
                break;
            }
        }

        HRESULT DXResource::Validate(){ return S_OK; }
        void DXResource::Invalidate(){}
    };
};