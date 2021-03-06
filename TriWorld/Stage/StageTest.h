﻿#pragma once

#include"JBF/JBFramework.h"

#include"Config/Config.h"

#include"Base/Base.h"
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
    bool bGridSwitch;
    bool bAxisSwitch;

private:
    CfgParseINI cfgINIReader;

private:
    D3DLIGHT9 envLight;

private:
    ObjCamera* objCamera;
    ObjFont* objFont;

private:
    ObjGrid* objGrid;
    ObjAxis* objAxis;

private:
    ObjPlane* objPlane;
    ObjDwarf* objDwarf;
};