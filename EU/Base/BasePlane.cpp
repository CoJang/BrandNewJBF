#include"pch.h"
#include"Base.h"

const D3DVERTEXELEMENT9 BasePlane::ins_vertElememt[] = {
    { 0, 0, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
};
const BasePlane::INDEX BasePlane::ins_indRaw[] = {
    { 0u, 1u, 2u },
    { 2u, 3u, 0u }
};

BasePlane::BasePlane() : Base::DXResource(RESTYPE_MANAGE), ins_vertBuf(nullptr), ins_indBuf(nullptr), ins_vertDecl(nullptr){}
BasePlane::~BasePlane(){ Invalidate(); }

BasePlane* BasePlane::Create(const D3DXVECTOR2* size){
    auto _new = new BasePlane();

    /*
    v0---v1
    |\    |
    | \ t0|
    |  \  |
    |t1 \ |
    |    \|
    v3---v2
    */
    _new->ins_vertRaw[0] = { { 0.f, 0.f, 0.f }, { 0.f, 0.f } };
    _new->ins_vertRaw[1] = { { size->x, 0.f, 0.f }, { 1.f, 0.f } };
    _new->ins_vertRaw[2] = { { size->x, -size->y, 0.f }, { 1.f, 1.f } };
    _new->ins_vertRaw[3] = { { 0.f, -size->y, 0.f }, { 0.f, 1.f } };

    if (FAILED(_new->Validate()))return nullptr;

    return _new;
}
void BasePlane::Release(){ delete this; }

HRESULT BasePlane::Validate(){
    void* buf;
    HRESULT hr;

    {
        hr = Core::Graphic::CreateVertexBuffer(
            sizeof ins_vertRaw,
            D3DUSAGE_WRITEONLY,
            VERTEX::FVF,
            D3DPOOL_MANAGED,
            &ins_vertBuf,
            nullptr
        );
        if (FAILED(hr))return hr;

        {
            hr = ins_vertBuf->Lock(0, sizeof ins_vertRaw, &buf, NULL);
            if (FAILED(hr))return hr;

            memcpy_s(buf, sizeof ins_vertRaw, ins_vertRaw, sizeof ins_vertRaw);

            ins_vertBuf->Unlock();
        }
    }

    {
        hr = Core::Graphic::CreateIndexBuffer(
            sizeof ins_indRaw,
            D3DUSAGE_WRITEONLY,
            INDEX::FIF,
            D3DPOOL_MANAGED,
            &ins_indBuf,
            nullptr
        );
        if (FAILED(hr))return hr;

        {
            hr = ins_indBuf->Lock(0, sizeof ins_vertRaw, &buf, NULL);
            if (FAILED(hr))return hr;

            memcpy_s(buf, sizeof ins_indRaw, ins_indRaw, sizeof ins_indRaw);

            ins_indBuf->Unlock();
        }
    }

    {
        hr = Core::Graphic::CreateVertexDeclaration(
            ins_vertElememt,
            &ins_vertDecl
        );
        if (FAILED(hr))return hr;
    }

    return S_OK;
}
void BasePlane::Invalidate(){
    if (ins_vertBuf)RELEASE(ins_vertBuf);
    if (ins_indBuf)RELEASE(ins_indBuf);
    if (ins_vertDecl)RELEASE(ins_vertDecl);
}

void BasePlane::SendFaceInfo(){
    Core::Graphic::SetStreamSource(0, ins_vertBuf, 0, sizeof(decltype(*ins_vertRaw)));
    Core::Graphic::SetIndices(ins_indBuf);
    Core::Graphic::SetVertexDeclaration(ins_vertDecl);
}
HRESULT BasePlane::Draw(){
    return Core::Graphic::DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _countof(ins_vertRaw), 0, _countof(ins_indRaw));
}