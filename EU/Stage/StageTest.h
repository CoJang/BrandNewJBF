#pragma once

#include"JBF/JBFramework.h"

#include"Object/Object.h"

using namespace JBF;

class StageTest : public Base::Stage{
public:
    StageTest(){}
    virtual ~StageTest(){}

public:
    void Init();
    void Cleanup();

private:
    void ins_initFace();
    void ins_initFrame();
    void ins_initObject();

private:
    void ins_releaseFace();
    void ins_releaseFrame();
    void ins_releaseObject();

public:
    void Update(float delta);
    void Draw();

private:
    void ins_drawGame(const Matrix* matVP);

    void ins_drawTextureOriginal(const Matrix* matWVP, const Object::EmptyTexture* texture);
    void ins_drawTextureBrighRegion(const Matrix* matWVP, const float* fBrightPassLevel, const Object::EmptyTexture* texture);
    void ins_drawTextureBlurHorz(const Matrix* matWVP, const Object::EmptyTexture* texture);
    void ins_drawTextureBlurVert(const Matrix* matWVP, const Object::EmptyTexture* texture);
    void ins_drawTextureCombine(const Matrix* matWVP, const Object::EmptyTexture* texBase, const Object::EmptyTexture* texBloom);
    void ins_drawTextureDistortion(const Matrix* matWVP, const Object::EmptyTexture* texture);

private:
    float cfgBrightPassLevel;

private:
    Object::EmptyTexture* faceGame;
    Object::EmptyTexture* faceRenderPass[2];

    Matrix matFrame;
    Matrix matFrameDown2X;
    Matrix matFrameUp2X;

    BasePlane* sprFrame;

private:
    ObjTest* objBackground;
    ObjTest* objHuman;
};