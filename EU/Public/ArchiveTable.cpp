#include"pch.h"

#include"Public.h"

JBF::Global::Archive::Decrypter arcSprites;
JBF::Global::Archive::Decrypter arcShaders;

void ArchiveLoad(){
    if (!arcSprites.OpenFile(_T("./Content/EU_Sprites.jba")))return;
    if (!arcShaders.OpenFile(_T("./Content/EU_Shaders.jba")))return;
}
void ArchiveCleanup(){
    arcSprites.CloseFile();
    arcShaders.CloseFile();
}