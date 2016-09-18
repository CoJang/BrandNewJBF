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
    void ins_drawScene(const Matrix* matP);

private:
    Object::EmptyTexture* faceGame;

    Matrix matFrame;
    Object::Shader* sprShader;
    BasePlane* sprFrame;

private:
    ObjCamera* objCamera;

private:
    ObjHuman* objHuman;
};