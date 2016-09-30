#include"pch.h"

#include"Public.h"


#define SHADER_FILENAME(str) JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(str, tstrlen(str))


JBF::Object::Shader* shadBasic;
JBF::Object::Shader* shadRGBZero;

JBF::Object::Shader* shadLightExtract;
JBF::Object::Shader* shadLightCombine;

JBF::Object::Shader* shadBright;
JBF::Object::Shader* shadBlur;
JBF::Object::Shader* shadBloomCombine;

JBF::Object::Shader* shadDistortion;


void ShaderLoad(){
    if (!(shadBasic = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Basic.fxo")))))return;
    if (!(shadRGBZero = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("RGBZero.fxo")))))return;

    if (!(shadLightExtract = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("LightExtract.fxo")))))return;
    if (!(shadLightCombine = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("LightCombine.fxo")))))return;

    if (!(shadBright = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Bright.fxo")))))return;
    if (!(shadBlur = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Blur.fxo")))))return;
    if (!(shadBloomCombine = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("BloomCombine.fxo")))))return;

    if (!(shadDistortion = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("CubicDistortion.fxo")))))return;
}
void ShaderCleanup(){
    RELEASE(shadBasic);
    RELEASE(shadRGBZero);

    RELEASE(shadLightExtract);
    RELEASE(shadLightCombine);

    RELEASE(shadBright);
    RELEASE(shadBlur);
    RELEASE(shadBloomCombine);

    RELEASE(shadDistortion);
}