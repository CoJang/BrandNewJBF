#include"pch.h"

#include"Public/Public.h"
#include"Stage.h"


void StageTitle::ins_drawBackground(const Matrix* matVP){
    ins_objBackground->DrawBase();
}
void StageTitle::ins_drawBackgroundLightMask(const Matrix* matVP){
    ins_objBackground->DrawLightMask();
}
void StageTitle::ins_drawBackgroundLight(const Matrix* matVP){
    ins_objBackLight->DrawBase();
}

void StageTitle::ins_drawObject(const Matrix* matVP){
    ins_objPlayer->DrawBase();
}

void StageTitle::ins_drawForeground(const Matrix* matVP){
    ins_objSofa->DrawBase();
}
void StageTitle::ins_drawForegroundLightMask(const Matrix* matVP){
    ins_objSofa->DrawLightMask();
}
void StageTitle::ins_drawForegroundLight(const Matrix* matVP){
    ins_objFrontLight->DrawBase();
}