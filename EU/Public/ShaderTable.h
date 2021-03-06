﻿#pragma once

#include"JBF/JBFramework.h"


extern JBF::Object::Shader* shadBasic;
extern JBF::Object::Shader* shadRGBZero;

extern JBF::Object::Shader* shadLightExtract;
extern JBF::Object::Shader* shadLightCombine;

extern JBF::Object::Shader* shadBright;
extern JBF::Object::Shader* shadBlur;
extern JBF::Object::Shader* shadBloomCombine;

extern JBF::Object::Shader* shadDistortion;


extern void ShaderLoad();
extern void ShaderCleanup();