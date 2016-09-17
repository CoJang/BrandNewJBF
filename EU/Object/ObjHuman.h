#pragma once

#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjHuman{
private:
    BasePlane* ins_sprite;
    Object::ExternalTexture* ins_texture;

private:
    Object::Shader* ins_shader;

private:
    Matrix ins_world;

public:
    ObjHuman();
    virtual ~ObjHuman();

public:
    bool Draw();
};