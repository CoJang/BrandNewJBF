#include"pch.h"

#include"Public.h"
#include"Object/Object.h"

ObjCamera* objCamera;


void GlobalObjectInit(){
    if (!(objCamera = ObjCamera::Create()))return;
}
void GlobalObjectCleanup(){
    RELEASE(objCamera);
}