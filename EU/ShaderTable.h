#pragma once

class JBF::Object::Shader;


extern JBF::Object::Shader* shadBasicWrap;
extern JBF::Object::Shader* shadBasicClamp;

extern JBF::Object::Shader* shadBright;
extern JBF::Object::Shader* shadBlur;
extern JBF::Object::Shader* shadCombine;

extern JBF::Object::Shader* shadDistortion;


extern void ShaderLoad();
extern void ShaderCleanup();