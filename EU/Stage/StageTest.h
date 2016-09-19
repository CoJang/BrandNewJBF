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
    void ins_initShader();
    void ins_initFrame();
    void ins_initObject();

private:
    void ins_releaseFace();
    void ins_releaseShader();
    void ins_releaseFrame();
    void ins_releaseObject();

public:
    void Update(float delta);
    void Draw();

private:
    void ins_drawGame(const Matrix* matVP);

    void ins_drawTextureOriginal(const Matrix* matWMP, const Object::EmptyTexture* texture);
    void ins_drawTextureDownCast4X(const Matrix* matWMP, const Object::EmptyTexture* texture);
    void ins_drawTextureBrighRegion(const Matrix* matWMP, const float* fBrightPassLevel, const Object::EmptyTexture* texture);
    void ins_drawTextureBlurHorz(const Matrix* matWMP, const float* fBrightLevel, const Object::EmptyTexture* texture);
    void ins_drawTextureBlurVert(const Matrix* matWMP, const float* fBrightLevel, const Object::EmptyTexture* texture);
    void ins_drawTextureUpCast4X(const Matrix* matWMP, const Object::EmptyTexture* texture);
    void ins_drawTextureCombine4X(const Matrix* matWMP, const float* fSecondAlpha, const Object::EmptyTexture* textureA, const Object::EmptyTexture* textureB);

private:
    float cfgBrightPassLevel;
    float cfgBloomLevel;
    float cfgBloomAlpha;

private:
    Object::EmptyTexture* faceGame;
    Object::EmptyTexture* faceRenderPass[2];

    Object::Shader* shadBasic;
    Object::Shader* shadDowncast4X;
    Object::Shader* shadBright;
    Object::Shader* shadBlurHorz;
    Object::Shader* shadBlurVert;
    Object::Shader* shadUpcast4X;
    Object::Shader* shadCombine4X;

    Matrix matFrame;
    Matrix matFrameDown4X;
    Matrix matFrameUp4X;

    BasePlane* sprFrame;

private:
    ObjCamera* objCamera;

private:
    ObjTest* objBackground;
    ObjTest* objHuman;
};