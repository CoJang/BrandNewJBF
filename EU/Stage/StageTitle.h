#pragma once

#include"JBF/JBFramework.h"

#include"Object/Object.h"

using namespace JBF;

class StageTitle : public Base::Stage{
public:
    StageTitle(){}
    virtual ~StageTitle(){}

public:
    void Init();
    HRESULT Validate();
    void Cleanup();
    void Invalidate();
private:
    void ins_initMatrix();
    void ins_initFace();
    void ins_initFrame();
    void ins_initObject();
private:
    void ins_releaseMatrix();
    void ins_releaseFace();
    void ins_releaseFrame();
    void ins_releaseObject();

public:
    void Update(float delta);
    void Draw();
private:
    void ins_drawBackground(const Matrix* matVP);
    void ins_drawBackgroundLightMask(const Matrix* matVP);
    void ins_drawBackgroundLight(const Matrix* matVP);

    void ins_drawObject(const Matrix* matVP);

    void ins_drawForeground(const Matrix* matVP);
    void ins_drawForegroundLightMask(const Matrix* matVP);
    void ins_drawForegroundLight(const Matrix* matVP);

private:
    void ins_drawTextureOriginal(const Matrix* matWVP, const Object::EmptyTexture* texture);
    void ins_drawTextureRGBZero(const Matrix* matWVP, const Object::EmptyTexture* texture);

    void ins_drawTextureLightExtract(const Matrix* matWVP, const Object::EmptyTexture* texLight, const Object::EmptyTexture* texMask);
    void ins_drawTextureLightCombine(const Matrix* matWVP, const Object::EmptyTexture* texBase, const Object::EmptyTexture* texBackLight, const Object::EmptyTexture* texFrontLight);

    void ins_drawTextureUpDownFilter(const Matrix* matWVP, const Object::EmptyTexture* texture);
    
    void ins_drawTextureBrighRegion(const Matrix* matWVP, const Object::EmptyTexture* texBase, const Object::EmptyTexture* texBackLight, const Object::EmptyTexture* texFrontLight);

    void ins_drawTextureBlurHorz(const Matrix* matWVP, const Object::EmptyTexture* texture);
    void ins_drawTextureBlurVert(const Matrix* matWVP, const Object::EmptyTexture* texture);

    void ins_drawTextureBloomCombine(const Matrix* matWVP, const Object::EmptyTexture* texBase, const Object::EmptyTexture* texBloom);

private:
    Matrix ins_matFrame;
    Matrix ins_matFrameDown2X;
    Matrix ins_matFrameUp2X;

private:
    BasePlane* ins_sprFrame;

private:
    Object::EmptyTexture* ins_faceObject;
    Object::EmptyTexture* ins_faceGame;
    Object::EmptyTexture* ins_faceLightMask[2];
    Object::EmptyTexture* ins_faceLight[3];

private:
    ObjBackground* ins_objBackground;
    ObjLight* ins_objBackLight;
};