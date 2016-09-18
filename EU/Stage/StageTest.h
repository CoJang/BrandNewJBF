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

public:
    void Update(float delta);
    void Draw();

private:
    void ins_drawGame(const Matrix* matVP);

private:
    Object::EmptyTexture* faceGame;

private:
    ObjCamera* objCamera;

private:
    ObjHuman* objHuman;
};