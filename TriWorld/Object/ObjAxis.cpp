#include"pch.h"
#include"Object.h"

ObjAxis::ObjAxis() : DXResource(RESTYPE_MANAGE){}
ObjAxis::~ObjAxis(){
    Invalidate();
}

ObjAxis* ObjAxis::Create(float length){
    auto _new = Global::Alloc::NewCustomAligned<ObjAxis>(32);

    _new->Init(length);
    return _new;
}
void ObjAxis::Release(){ Global::Alloc::DeleteCustomAligned(this); }

HRESULT ObjAxis::Validate(){
    FORMAT_VERTEX* buf;
    HRESULT hr;

    hr = Core::Graphic::CreateVertexBuffer(
        sizeof ins_vertices,
        D3DUSAGE_WRITEONLY,
        FORMAT_VERTEX::FVF,
        D3DPOOL_MANAGED,
        &ins_vBuf,
        nullptr
    );
    if (FAILED(hr))return hr;

    hr = ins_vBuf->Lock(0, sizeof ins_vertices, (void**)&buf, NULL);
    if (FAILED(hr))return hr;

    memcpy_s(buf, sizeof ins_vertices, ins_vertices, sizeof ins_vertices);

    ins_vBuf->Unlock();
    return S_OK;
}
void ObjAxis::Invalidate(){
    if (ins_vBuf)RELEASE(ins_vBuf);
}

void ObjAxis::Init(float length){
    ins_mWorld.identify();

    { // 라인 3개 + 글자 출력 좌표 3개
        // z축 라인
        ins_vertices[0].pos = D3DXVECTOR3(0, 0, 0);
        ins_vertices[1].pos = D3DXVECTOR3(0, 0, length);
        ins_vertices[0].color = D3DXCOLOR(0, 0.3f, 1, 1);
        ins_vertices[1].color = D3DXCOLOR(0, 0.3f, 1, 1);

        // x축 라인
        ins_vertices[2].pos = D3DXVECTOR3(0, 0, 0);
        ins_vertices[3].pos = D3DXVECTOR3(length, 0, 0);
        ins_vertices[2].color = D3DXCOLOR(1, 0, 0, 1);
        ins_vertices[3].color = D3DXCOLOR(1, 0, 0, 1);

        // y축 라인
        ins_vertices[4].pos = D3DXVECTOR3(0, 0, 0);
        ins_vertices[5].pos = D3DXVECTOR3(0, length, 0);
        ins_vertices[4].color = D3DXCOLOR(0, 1, 0, 1);
        ins_vertices[5].color = D3DXCOLOR(0, 1, 0, 1);
    }

    {
        ins_vTexPos[0] = Vector3(0, 0, length + 0.1f);
        ins_vTexPos[1] = Vector3(length + 0.1f, 0, 0);
        ins_vTexPos[2] = Vector3(0, length + 0.1f, 0);
    }

    Validate();
}

void ObjAxis::Update(Vector3* vpPos){
    {
        ins_mWorld._41 = vpPos->x;
        ins_mWorld._42 = vpPos->y;
        ins_mWorld._43 = vpPos->z;
    }

    {
        D3DVIEWPORT9 viewPort;
        Matrix mTmp;
        Matrix mText = ins_mWorld;

        Core::Graphic::GetViewport(&viewPort);

        Matrix mViewPort = Matrix(
            viewPort.Width * 0.5f, 0, 0, 0,
            0, -(viewPort.Height * 0.5f), 0, 0,
            0, 0, 1, 0,
            viewPort.Width * 0.5f, viewPort.Height - (viewPort.Height * 0.5f), 0, 1
        );

        Core::Graphic::GetTransform(D3DTS_VIEW, &mTmp);
        mText *= mTmp;

        Core::Graphic::GetTransform(D3DTS_PROJECTION, &mTmp);
        mText *= mTmp;
        
        mText *= mViewPort;

        Global::Math::Vec3TransformCoord(&ins_vTexPos[3], &ins_vTexPos[0], &mText);
        Global::Math::Vec3TransformCoord(&ins_vTexPos[4], &ins_vTexPos[1], &mText);
        Global::Math::Vec3TransformCoord(&ins_vTexPos[5], &ins_vTexPos[2], &mText);
    }
}
void ObjAxis::Draw(){
    DWORD lightState;
    DWORD zenableState;

    Core::Graphic::SetTransform(D3DTS_WORLD, &ins_mWorld);

    Core::Graphic::GetRenderState(D3DRS_LIGHTING, &lightState);
    Core::Graphic::GetRenderState(D3DRS_ZENABLE, &zenableState);

    Core::Graphic::SetRenderState(D3DRS_LIGHTING, FALSE);
    Core::Graphic::SetRenderState(D3DRS_ZENABLE, FALSE);

    Core::Graphic::SetTexture(0, nullptr);
    Core::Graphic::SetTexture(1, nullptr);
    Core::Graphic::SetTexture(2, nullptr);

    Core::Graphic::SetVertexShader(nullptr);
    Core::Graphic::SetPixelShader(nullptr);

    Core::Graphic::SetFVF(FORMAT_VERTEX::FVF);
    Core::Graphic::SetStreamSource(0, ins_vBuf, 0, sizeof FORMAT_VERTEX);

    Core::Graphic::DrawPrimitive(D3DPT_LINELIST, 0, 3);

    Core::Graphic::SetRenderState(D3DRS_LIGHTING, lightState);
    Core::Graphic::SetRenderState(D3DRS_ZENABLE, zenableState);

    ObjFont::GetInstance()->Draw(ins_vTexPos[3].x, ins_vTexPos[3].y, ins_vertices[0].color, _T("Z"));
    ObjFont::GetInstance()->Draw(ins_vTexPos[4].x, ins_vTexPos[4].y, ins_vertices[2].color, _T("X"));
    ObjFont::GetInstance()->Draw(ins_vTexPos[5].x, ins_vTexPos[5].y, ins_vertices[4].color, _T("Y"));
}