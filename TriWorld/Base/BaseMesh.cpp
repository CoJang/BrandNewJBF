#include"pch.h"

#include"BaseMesh.h"
#include"ArchiveTable.h"

BaseMesh::BaseMesh() : Base::DXResource(RESTYPE_MANAGE){
    ins_mesh = nullptr;
}
BaseMesh::~BaseMesh(){

}

bool BaseMesh::ParseXFormat(ARCHIVE_HASHSIZE fileName){
    {
        auto pos = arcModels.GetIterator(fileName);
        DWORD size;

        if (!arcModels.GetSize(pos, &size))return false;
        ins_rawData.resize(size);

        if (!arcModels.GetData(pos, &ins_rawData[0]))return false;
    }
    ins_validateFunc = ins_fillMeshByXFormat;

    return SUCCEEDED(ins_validateFunc(this));
}

HRESULT BaseMesh::Validate(){ return ins_validateFunc(this); }
void BaseMesh::Invalidate(){
    if (ins_mesh)RELEASE(ins_mesh);
}

HRESULT BaseMesh::ins_fillMeshByXFormat(BaseMesh* obj){
    HRESULT hr;

    ID3DXBuffer* bufMtrl;
    DWORD sizeMtrl;

    hr = D3DXLoadMeshFromXInMemory(
        &obj->ins_rawData[0], obj->ins_rawData.size(),
        D3DXMESH_MANAGED,
        Core::Graphic::GetDevice(),
        nullptr,
        &bufMtrl,
        nullptr,
        &sizeMtrl,
        &obj->ins_mesh
    );
    ASSERT_HRESULT(hr, _T("Failed to load XFile mesh."));

    obj->ins_mtrlTable.resize(sizeMtrl);
    for (auto i = decltype(sizeMtrl){0}; i < sizeMtrl; ++i){
        D3DXMATERIAL* curMtrl = (decltype(curMtrl))bufMtrl + i;
        ARCHIVE_HASHSIZE key = Global::Hash::X65599Generator<decltype(key), char>((const char*)curMtrl->pTextureFilename, strlen(curMtrl->pTextureFilename));

        obj->ins_mtrlTable[i] = curMtrl->MatD3D;
    }

    return S_OK;
}