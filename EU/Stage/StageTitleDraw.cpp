#include"pch.h"

#include"Public/Public.h"
#include"Stage.h"


void StageTitle::ins_drawBackground(const Matrix* matVP){
    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    ins_objBackground->DrawBase(matVP);
}
void StageTitle::ins_drawBackgroundLightMask(const Matrix* matVP){
    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    ins_objBackground->DrawLightMask(matVP);
}
void StageTitle::ins_drawBackgroundLight(const Matrix* matVP){

}

void StageTitle::ins_drawObject(const Matrix* matVP){
    Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


}

void StageTitle::ins_drawForeground(const Matrix* matVP){

}
void StageTitle::ins_drawForegroundLightMask(const Matrix* matVP){

}
void StageTitle::ins_drawForegroundLight(const Matrix* matVP){

}