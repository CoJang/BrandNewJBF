#include"pch.h"

#include"JBF/JBFramework.h"
#include"ShaderTable.h"
#include"ArchiveTable.h"


#define SHADER_FILENAME(str) JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(str, tstrlen(str))


JBF::Object::Shader* shadBasicWrap;
JBF::Object::Shader* shadBasicClamp;

JBF::Object::Shader* shadBright;
JBF::Object::Shader* shadBlur;
JBF::Object::Shader* shadCombine;

JBF::Object::Shader* shadDistortion;


void ShaderLoad(){
    if (!(shadBasicWrap = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Basic_wrap.fxo")))))return;
    if (!(shadBasicClamp = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Basic_clamp.fxo")))))return;

    if (!(shadBright = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Bright.fxo")))))return;
    if (!(shadBlur = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Blur.fxo")))))return;
    if (!(shadCombine = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Combine.fxo")))))return;

    if (!(shadDistortion = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("CubicDistortion.fxo")))))return;
}
void ShaderCleanup(){
    RELEASE(shadBasicWrap);
    RELEASE(shadBasicClamp);

    RELEASE(shadBright);
    RELEASE(shadBlur);
    RELEASE(shadCombine);

    RELEASE(shadDistortion);
}