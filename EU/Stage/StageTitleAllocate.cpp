#include"pch.h"

#include"Public/Public.h"
#include"Stage.h"


#define RES_FILENAME(str) JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(str, tstrlen(str))


void StageTitle::ins_initMatrix(){
    Vector2 size = Vector2(Core::Graphic::GetDisplayInfo()->Width, Core::Graphic::GetDisplayInfo()->Height);

    ins_matFrame = Matrix(
        2 / size.x, 0, 0, 0,
        0, 2 / size.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );
    ins_matFrameDown2X = Matrix(
        1 / size.x, 0, 0, 0,
        0, 1 / size.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );
    ins_matFrameUp2X = Matrix(
        4 / size.x, 0, 0, 0,
        0, 4 / size.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );
}
void StageTitle::ins_initFace(){
    Object::EmptyTexture::INFO inf;

    {
        inf.pool = D3DPOOL_DEFAULT;
        inf.usage = D3DUSAGE_RENDERTARGET;
        inf.format = D3DFMT_A8R8G8B8;
        inf.mipLevels = 1;
        inf.width = Core::Graphic::GetDisplayInfo()->Width;
        inf.height = Core::Graphic::GetDisplayInfo()->Height;

        ins_faceObject = Object::EmptyTexture::Create(&inf);
        ins_faceObject->SetValidateCallback(
            [](void* arg)->void{
                auto self = reinterpret_cast<Object::EmptyTexture*>(arg);
                Object::EmptyTexture::INFO inf;

                memcpy_s(&inf, sizeof inf, self->GetInfo(), sizeof inf);
                inf.width = Core::Graphic::GetDisplayInfo()->Width;
                inf.height = Core::Graphic::GetDisplayInfo()->Height;

                self->SetInfo(&inf);
            },
            ins_faceObject
        );
    }

    {
        inf.pool = D3DPOOL_DEFAULT;
        inf.usage = D3DUSAGE_RENDERTARGET;
        inf.format = D3DFMT_R8G8B8;
        inf.mipLevels = 1;
        inf.width = Core::Graphic::GetDisplayInfo()->Width;
        inf.height = Core::Graphic::GetDisplayInfo()->Height;

        ins_faceGame = Object::EmptyTexture::Create(&inf);
        ins_faceGame->SetValidateCallback(
            [](void* arg)->void{
                auto self = reinterpret_cast<Object::EmptyTexture*>(arg);
                Object::EmptyTexture::INFO inf;

                memcpy_s(&inf, sizeof inf, self->GetInfo(), sizeof inf);
                inf.width = Core::Graphic::GetDisplayInfo()->Width;
                inf.height = Core::Graphic::GetDisplayInfo()->Height;

                self->SetInfo(&inf);
            },
            ins_faceGame
        );
    }

    {
        inf.pool = D3DPOOL_DEFAULT;
        inf.usage = D3DUSAGE_RENDERTARGET;
        inf.format = D3DFMT_R8G8B8;
        inf.mipLevels = 1;
        inf.width = Core::Graphic::GetDisplayInfo()->Width;
        inf.height = Core::Graphic::GetDisplayInfo()->Height;

        for (size_t i = 0; i < _countof(ins_faceLightMask); ++i){
            ins_faceLightMask[i] = Object::EmptyTexture::Create(&inf);
            ins_faceLightMask[i]->SetValidateCallback(
                [](void* arg)->void{
                    auto self = reinterpret_cast<Object::EmptyTexture*>(arg);
                    Object::EmptyTexture::INFO inf;

                    memcpy_s(&inf, sizeof inf, self->GetInfo(), sizeof inf);
                    inf.width = Core::Graphic::GetDisplayInfo()->Width;
                    inf.height = Core::Graphic::GetDisplayInfo()->Height;

                    self->SetInfo(&inf);
                },
                ins_faceLightMask[i]
            );
        }
    }
    {
        inf.pool = D3DPOOL_DEFAULT;
        inf.usage = D3DUSAGE_RENDERTARGET;
        inf.format = D3DFMT_R8G8B8;
        inf.mipLevels = 1;
        inf.width = Core::Graphic::GetDisplayInfo()->Width;
        inf.height = Core::Graphic::GetDisplayInfo()->Height;

        for (size_t i = 0; i < _countof(ins_faceLight); ++i){
            ins_faceLight[i] = Object::EmptyTexture::Create(&inf);
            ins_faceLight[i]->SetValidateCallback(
                [](void* arg)->void{
                    auto self = reinterpret_cast<Object::EmptyTexture*>(arg);
                    Object::EmptyTexture::INFO inf;

                    memcpy_s(&inf, sizeof inf, self->GetInfo(), sizeof inf);
                    inf.width = Core::Graphic::GetDisplayInfo()->Width;
                    inf.height = Core::Graphic::GetDisplayInfo()->Height;

                    self->SetInfo(&inf);
                },
                ins_faceLight[i]
            );
        }
    }
}
void StageTitle::ins_initFrame(){
    Vector2 size = Vector2(Core::Graphic::GetDisplayInfo()->Width, Core::Graphic::GetDisplayInfo()->Height);

    ins_sprFrame = BasePlane::Create(&size, &Vector2(-0.5f, -0.5f));
    ins_sprFrame->SetValidateCallback(
        [](void* arg)->void{
            Vector2 size = Vector2(Core::Graphic::GetDisplayInfo()->Width, Core::Graphic::GetDisplayInfo()->Height);
            auto self = reinterpret_cast<BasePlane*>(arg);
            self->Resize(&size, &Vector2(-0.5f, -0.5f));
        },
        ins_sprFrame
    );
}
void StageTitle::ins_initObject(){
    objCamera->Init();

    ins_objBackground = ObjBackground::Create(
        RES_FILENAME(_T("Background/title.png")),
        RES_FILENAME(_T("Background/title_mask.png"))
    );
    ins_objBackLight = ObjLight::Create(
        RES_FILENAME(_T("Light/title_backLight.png"))
    );
}

void StageTitle::ins_releaseMatrix(){

}
void StageTitle::ins_releaseFace(){
    RELEASE(ins_faceObject);
    RELEASE(ins_faceGame);
    for (size_t i = 0; i < _countof(ins_faceLightMask); ++i)RELEASE(ins_faceLightMask[i]);
    for (size_t i = 0; i < _countof(ins_faceLight); ++i)RELEASE(ins_faceLight[i]);
}
void StageTitle::ins_releaseFrame(){
    RELEASE(ins_sprFrame);
}
void StageTitle::ins_releaseObject(){
    RELEASE(ins_objBackground);
}