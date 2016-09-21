#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjGrid : Base::DXResource{
private:
    struct FORMAT_VERTEX{
        D3DXVECTOR3 pos;
        DWORD color;

        enum{ FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
    };

private:
    size_t ins_lineCount;
    FORMAT_VERTEX* ins_vertices;
    IDirect3DVertexBuffer9* ins_vBuf;

private:
    ObjGrid();
    virtual ~ObjGrid();

public:
    static ObjGrid* Create(size_t sideCellCount, float scale);
    void Release();

public:
    virtual HRESULT Validate();
    virtual void Invalidate();

public:
    void Init(size_t sideCellCount, float scale);
public:
    void Draw();
};