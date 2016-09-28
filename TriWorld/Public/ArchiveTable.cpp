#include"pch.h"

#include"ArchiveTable.h"

JBF::Global::Archive::Decrypter arcModels;
JBF::Global::Archive::Decrypter arcTextures;

void ArchiveLoad(){
    if (!arcModels.OpenFile(_T("./Content/TRI_Models.jba")))return;
    if (!arcTextures.OpenFile(_T("./Content/TRI_Textures.jba")))return;
}
void ArchiveCleanup(){
    arcModels.CloseFile();
    arcTextures.CloseFile();
}