#include"pch.h"

#include"BaseMesh.h"
#include"ArchiveTable.h"

BaseMesh::BaseMesh() : Base::DXResource(RESTYPE_MANAGE){
    ins_mesh = nullptr;
}
BaseMesh::~BaseMesh(){

}

bool BaseMesh::ParseXFormat(ARCHIVE_HASHSIZE fileName){
    ins_fileName = fileName;
    ins_validateFunc = ins_fillMeshByXFormat;

    return SUCCEEDED(ins_validateFunc(this));
}

HRESULT BaseMesh::Validate(){ return ins_validateFunc(this); }
void BaseMesh::Invalidate(){
    if (ins_mesh)RELEASE(ins_mesh);
}

HRESULT BaseMesh::ins_fillMeshByXFormat(BaseMesh* obj){
    HRESULT hr;

    void* bufData;
    DWORD sizeData;

    ID3DXBuffer* bufMtrl;
    DWORD sizeMtrl;


    if (arcModels.GetDataLock(obj->ins_fileName, &bufData, &sizeData)){
        hr = D3DXLoadMeshFromXInMemory(
            bufData, sizeData,
            D3DXMESH_MANAGED,
            Core::Graphic::GetDevice(),
            nullptr,
            &bufMtrl,
            nullptr,
            &sizeMtrl,
            &obj->ins_mesh
        );
        arcModels.Unlock();
        ASSERT_HRESULT(hr, _T("Failed to load XFile mesh."));

        obj->ins_mtrlTable.resize(sizeMtrl);
        for (auto i = decltype(sizeMtrl){0}; i < sizeMtrl; ++i){
            D3DXMATERIAL* curMtrl = (decltype(curMtrl))bufMtrl + i;
            ARCHIVE_HASHSIZE key = Global::Hash::X65599Generator<decltype(key), char>((const char*)curMtrl->pTextureFilename, strlen(curMtrl->pTextureFilename));

            obj->ins_mtrlTable[i] = curMtrl->MatD3D;
        }
    }
    else{
        ASSERT(false, _T("Failed to read data from archive."));
        return E_FAIL;
    }

    return S_OK;
}