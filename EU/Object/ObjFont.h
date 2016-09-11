#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjFont : Base::DXResource{
private:
    static ObjFont* ins_instance;

private:
    ID3DXFont* ins_font;

private:
    ObjFont();
    virtual ~ObjFont();

public:
    virtual HRESULT Validate();
    virtual void Invalidate();

public:
    void Init();

public:
    HRESULT Draw(int x, int y, DWORD color, TCHAR* str, ...);

public:
    static ObjFont* GetInstance();
    static void Release(ObjFont* inst);
};