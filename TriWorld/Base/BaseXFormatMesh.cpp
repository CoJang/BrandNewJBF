#include"pch.h"
#include"Base.h"

BaseXFormatMesh::BaseXFormatMesh() : BaseMesh(RESTYPE_MANAGE){}
BaseXFormatMesh::~BaseXFormatMesh(){ Invalidate(); }

HRESULT BaseXFormatMesh::Validate(){
    HRESULT hr;

    void* bufData;
    DWORD sizeData;

    ID3DXBuffer* bufMtrl;
    D3DXMATERIAL* convMtrl;

    if (ins_arcModel->GetDataLock(ins_fileName, &bufData, &sizeData)){
        hr = D3DXLoadMeshFromXInMemory(
            bufData, sizeData,
            D3DXMESH_MANAGED,
            Core::Graphic::GetDevice(),
            nullptr,
            &bufMtrl,
            nullptr,
            &ins_partConut,
            &ins_mesh
        );
        ins_arcModel->Unlock();
        ASSERT_HRESULT(hr, _T("Failed to load XFile mesh.\nArchive: %s\nHash key: %u\nError code: %x"), ins_arcModel->GetFilePath(), ins_fileName, hr);

        {
            ins_mtrlTable.resize(ins_partConut);
            ins_texTable.resize(ins_partConut);
        }

        convMtrl = (decltype(convMtrl))bufMtrl->GetBufferPointer();

        for (auto i = decltype(ins_partConut){0}; i < ins_partConut; ++i){
            if (convMtrl[i].pTextureFilename){
                ARCHIVE_HASHSIZE key = Global::Hash::X65599Generator<decltype(key), char>((const char*)convMtrl[i].pTextureFilename, strlen(convMtrl[i].pTextureFilename));
                ins_texTable[i] = Object::ExternalTexture::Read(ins_arcTexture, key);
            }

            ins_mtrlTable[i] = convMtrl[i].MatD3D;
            {
                ins_mtrlTable[i].Diffuse.a = ins_mtrlTable[i].Diffuse.a <= 0.f ? 1.f : ins_mtrlTable[i].Diffuse.a;
                ins_mtrlTable[i].Ambient = ins_mtrlTable[i].Diffuse;
            }
        }

        RELEASE(bufMtrl);
    }
    else{
        ASSERT(false, _T("Failed to read data from archive.\nArchive: %s\nHash key: %u"), ins_arcModel->GetFilePath(), ins_fileName);
        return E_FAIL;
    }

    return S_OK;
}
void BaseXFormatMesh::Invalidate(){
    for (auto i = ins_texTable.begin(), e = ins_texTable.end(); i != e; ++i){
        auto cur = *i;
        if (cur){
            RELEASE(cur);
            *i = nullptr;
        }
    }

    if(ins_mesh)RELEASE(ins_mesh);
}