#include"pch.h"

#include"Public.h"


#define SHADER_FILENAME(str) JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(str, tstrlen(str))


JBF::Object::Shader* shadLight;


void ShaderLoad(){
    if (!(shadLight = JBF::Object::Shader::Read(&arcShaders, SHADER_FILENAME(_T("Light.fxo")))))return;
}
void ShaderCleanup(){
    RELEASE(shadLight);
}