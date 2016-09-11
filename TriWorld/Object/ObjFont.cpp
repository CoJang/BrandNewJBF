#include"pch.h"
#include"ObjFont.h"

ObjFont* ObjFont::ins_instance = nullptr;

ObjFont::ObjFont() : DXResource(RESTYPE_VRAM){}

HRESULT ObjFont::Validate(){
    auto hr = D3DXCreateFont(
        Core::Graphic::GetDevice(),
        12, // 세로
        6, // 가로
        1, // 두께
        0, // Mip Level
        false, // bUseItalic,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        _T("돋움"),
        &ins_font
    );

    if (FAILED(hr))return hr;
    return S_OK;
}
void ObjFont::Invalidate(){
    if (ins_font)RELEASE(ins_font);
}

void ObjFont::Init(){
    Validate();
}
ObjFont::~ObjFont(){
    Invalidate();
}

HRESULT ObjFont::Draw(int x, int y, DWORD color, TCHAR* str, ...){
    RECT rt = { x, y, 0, 0 };
    TCHAR buf[1024];
    va_list iterat;

    va_start(iterat, str);
    vstprintf_s(buf, str, iterat);
    va_end(iterat);

    return ins_font->DrawText(nullptr, buf, tstrlen(buf), &rt, DT_NOCLIP, color);
}

ObjFont* ObjFont::GetInstance(){
    if (!ins_instance)ins_instance = new ObjFont();
    return ins_instance;
}
void ObjFont::Release(ObjFont* inst){
    delete ins_instance;
    ins_instance = nullptr;
}