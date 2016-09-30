#include"pch.h"

#include"Public/Public.h"
#include"Stage.h"


void StageTitle::ins_drawBackground(const Matrix* matVP){
    ins_objBackground->DrawBase(matVP);
}
void StageTitle::ins_drawBackgroundLightMask(const Matrix* matVP){
    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    ins_objBackground->DrawLightMask(matVP);
}
void StageTitle::ins_drawBackgroundLight(const Matrix* matVP){
    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    ins_objBackLight->DrawBase(matVP);
}

void StageTitle::ins_drawObject(const Matrix* matVP){
    
}

void StageTitle::ins_drawForeground(const Matrix* matVP){

}
void StageTitle::ins_drawForegroundLightMask(const Matrix* matVP){

}
void StageTitle::ins_drawForegroundLight(const Matrix* matVP){

}