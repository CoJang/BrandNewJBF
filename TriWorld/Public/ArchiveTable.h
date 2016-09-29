#pragma once

#include"JBF/JBFramework.h"


extern JBF::Global::Archive::Decrypter arcModels;
extern JBF::Global::Archive::Decrypter arcTextures;
extern JBF::Global::Archive::Decrypter arcShaders;


extern void ArchiveLoad();
extern void ArchiveCleanup();