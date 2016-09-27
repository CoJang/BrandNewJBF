#include"pch.h"

#include"JBF/JBFramework.h"
#include"Public.h"

#include"Object/Object.h"

ObjCamera* objCamera;


void GloalObjectInit(){
    if (!(objCamera = ObjCamera::Create()))return;
}
void GlobalObjectCleanup(){
    RELEASE(objCamera);
}