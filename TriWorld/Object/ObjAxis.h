#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjAxis : Base::DXResource{
private:
    struct FORMAT_VERTEX{
        D3DXVECTOR3 pos;
        DWORD color;

        enum{ FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
    };

private:
    FORMAT_VERTEX ins_vertices[6];
    Vector3 ins_vTexPos[6];
    Matrix ins_mWorld;
    IDirect3DVertexBuffer9* ins_vBuf;

public:
    ObjAxis();
    virtual ~ObjAxis();

public:
    virtual HRESULT Validate();
    virtual void Invalidate();

public:
    void Init(float length);
public:
    void Update(Vector3* vpPos);
    void Draw();
};