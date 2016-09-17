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
    ObjCamera* objCamera;

private:
    ObjHuman* objHuman;
};