#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;

class StageTitle : public Base::Stage{
public:
    StageTitle(){}
    virtual ~StageTitle(){}

public:
    void Init();
    void Cleanup();

public:
    void Update(float delta);
    void Draw();
};