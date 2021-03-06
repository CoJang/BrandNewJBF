﻿#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;

class BasePlane : Base::DXResource{
private:
    struct VERTEX{
        D3DXVECTOR3 pos;
        D3DXVECTOR2 uv;

        static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
    };
    struct INDEX{
        WORD _0, _1, _2;

        static const D3DFORMAT FIF = D3DFMT_INDEX16;
    };

private:
    VERTEX ins_vertRaw[4];
    static const INDEX ins_indRaw[];
    static const D3DVERTEXELEMENT9 ins_vertElememt[];

private:
    IDirect3DVertexBuffer9* ins_vertBuf;
    IDirect3DIndexBuffer9* ins_indBuf;
    IDirect3DVertexDeclaration9* ins_vertDecl;

private:
    void(*ins_validateCallback)(void* arg);
    void* ins_validateCallbackArg;

private:
    BasePlane();
    virtual ~BasePlane();

public:
    static BasePlane* Create(const Global::Math::Vector2* size, const Global::Math::Vector2* offset);
    static INLINE BasePlane* Create(const Global::Math::Vector2* size){ return Create(size, &Global::Math::Vector2(0, 0)); }
    void Release();

public:
    void Resize(const Global::Math::Vector2* size, const Global::Math::Vector2* offset);
    INLINE void Resize(const Global::Math::Vector2* size){ Resize(size, &Global::Math::Vector2(0, 0)); }

public:
    virtual HRESULT Validate();
    virtual void Invalidate();

public:
    void SendFaceInfo();
    HRESULT Draw();

public:
    INLINE void SetValidateCallback(void(*callback)(void* arg), void* arg){ ins_validateCallback = callback; ins_validateCallbackArg = arg; }
};