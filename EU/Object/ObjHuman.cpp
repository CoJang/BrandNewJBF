#include"pch.h"
#include"Object.h"

#include"ArchiveTable.h"
#define FILE_NAME _T("dummyHuman.png")

ObjHuman::ObjHuman(){
    D3DXVECTOR2 planSize;

    ins_texture = Object::ExternalTexture::Read(&arcSprites, Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(FILE_NAME, tstrlen(FILE_NAME)));
    planSize.x = ins_texture->GetInfo()->Width;
    planSize.y = ins_texture->GetInfo()->Height;

    ins_sprite = BasePlane::Create(&planSize);
}
ObjHuman::~ObjHuman(){
    Object::ExternalTexture::Release(ins_texture);
    BasePlane::Release(ins_sprite);
}

bool ObjHuman::Draw(){
    Core::Graphic::SetTransform(D3DTS_WORLD, &Matrix::constIdentity);


}