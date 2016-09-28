#include"pch.h"
#include"Object.h"

ObjDwarf::ObjDwarf(){}
ObjDwarf::~ObjDwarf(){}

ObjDwarf* ObjDwarf::Create(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName){
    auto _new = Global::Alloc::NewCustomAligned<ObjDwarf>(32);

    _new->Init(arcModel, arcTexture, fileName);
    return _new;
}
void ObjDwarf::Release(){ Global::Alloc::DeleteCustomAligned(this); }

void ObjDwarf::Init(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName){
    ins_arcModel = arcModel;
    ins_arcTexture = arcTexture;
    ins_fileName = fileName;

    ins_mWorld = Matrix::constIdentity;
    ins_mWorld._11 = ins_mWorld._22 = ins_mWorld._33 = 10.f;

    Validate();
}

void ObjDwarf::Update(float delta){

}
void ObjDwarf::Draw(){
    auto drawPartFunc = [=](DWORD i)->void{
        auto curTex = ins_texTable[i];

        Core::Graphic::SetMaterial(&ins_mtrlTable[i]);
        Core::Graphic::SetTexture(0, curTex ? curTex->GetTexture() : nullptr);

        ins_mesh->DrawSubset(i);
    };

    Core::Graphic::SetTransform(D3DTS_WORLD, &ins_mWorld);

    for (auto i = decltype(ins_partConut){0}; i < ins_partConut; ++i){
        drawPartFunc(i);
    }
}