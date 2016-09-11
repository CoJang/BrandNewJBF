﻿#pragma once

#include"JBF/JBFramework.h"

#include"Object/ObjCamera.h"
#include"Object/ObjFont.h"
#include"Object/ObjGrid.h"
#include"Object/ObjAxis.h"

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
    bool bGridSwitch;
    bool bAxisSwitch;

private:
    ObjCamera* objCamera;
    ObjFont* objFont;

    ObjGrid* objGrid;
    ObjAxis* objAxis;
};