#include"pch.h"
#include"ObjGrid.h"

ObjGrid::ObjGrid() : DXResource(RESTYPE_VRAM){}

HRESULT ObjGrid::Validate(){
    size_t assignSize = ins_lineCount * 2 * sizeof FORMAT_VERTEX;
    FORMAT_VERTEX* buf;
    HRESULT hr;

    hr = Core::Graphic::CreateVertexBuffer(
        assignSize,
        D3DUSAGE_WRITEONLY,
        FORMAT_VERTEX::FVF,
        D3DPOOL_DEFAULT,
        &ins_vBuf,
        nullptr
    );
    if (FAILED(hr))return hr;

    hr = ins_vBuf->Lock(0, assignSize, (void**)&buf, NULL);
    if (FAILED(hr))return hr;

    memcpy_s(buf, assignSize, ins_vertices, assignSize);

    ins_vBuf->Unlock();
    return S_OK;
}
void ObjGrid::Invalidate(){
    if (ins_vBuf)RELEASE(ins_vBuf);
}

void ObjGrid::Init(size_t sideCellCount, float scale){
    scale *= 0.5f;

    sideCellCount = sideCellCount % 2 ? sideCellCount + 1 : sideCellCount;
    ++sideCellCount;

    ins_lineCount = sideCellCount * sideCellCount;
    ins_vertices = new FORMAT_VERTEX[ins_lineCount * 2];

    int mid = sideCellCount >> 1;
    int i;
    auto pV = ins_vertices;

    for (i = 0; i < sideCellCount; i += 2){
        if (i == mid)continue;

        pV->pos = D3DXVECTOR3(i - mid, 0, -mid) * scale;
        (pV + 1)->pos = D3DXVECTOR3(i - mid, 0, sideCellCount - 1 - mid) * scale;

        pV->color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
        (pV + 1)->color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);

        pV += 2;
    }
    for (i = 0; i < sideCellCount; i += 2){
        if (i == mid)continue;

        pV->pos = D3DXVECTOR3(-mid, 0.f, i - mid) * scale;
        (pV + 1)->pos = D3DXVECTOR3(sideCellCount - 1 - mid, 0, i - mid) * scale;

        pV->color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
        (pV + 1)->color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);

        pV += 2;
    }
    {
        {
            pV->pos = D3DXVECTOR3(0, 0, -mid) * scale;
            (pV + 1)->pos = D3DXVECTOR3(0, 0, sideCellCount - 1 - mid) * scale;

            pV->color = D3DXCOLOR(0, 0, 0, 1);
            (pV + 1)->color = D3DXCOLOR(0, 0, 0, 1);
        }

        pV += 2;

        {
            pV->pos = D3DXVECTOR3(-mid, 0, 0) * scale;
            (pV + 1)->pos = D3DXVECTOR3(sideCellCount - 1 - mid, 0, 0) * scale;

            pV->color = D3DXCOLOR(0, 0, 0, 1);
            (pV + 1)->color = D3DXCOLOR(0, 0, 0, 1);
        }
    }

    Validate();
}
ObjGrid::~ObjGrid(){
    Invalidate();
    delete[] ins_vertices;
}

void ObjGrid::Draw(){
    DWORD lightState;
    DWORD specularState;
    DWORD zenableState;

    Core::Graphic::SetTransform(D3DTS_WORLD, &Matrix::constIdentity);

    Core::Graphic::GetRenderState(D3DRS_LIGHTING, &lightState);
    Core::Graphic::GetRenderState(D3DRS_SPECULARENABLE, &specularState);
    Core::Graphic::GetRenderState(D3DRS_ZENABLE, &zenableState);

    Core::Graphic::SetRenderState(D3DRS_LIGHTING, FALSE);
    Core::Graphic::SetRenderState(D3DRS_SPECULARENABLE, FALSE);
    Core::Graphic::SetRenderState(D3DRS_ZENABLE, FALSE);

    Core::Graphic::SetTexture(0, nullptr);
    Core::Graphic::SetTexture(1, nullptr);

    Core::Graphic::SetFVF(FORMAT_VERTEX::FVF);
    Core::Graphic::SetStreamSource(0, ins_vBuf, 0, sizeof FORMAT_VERTEX);

    Core::Graphic::DrawPrimitive(D3DPT_LINELIST, 0, ins_lineCount);

    Core::Graphic::SetRenderState(D3DRS_LIGHTING, lightState);
    Core::Graphic::SetRenderState(D3DRS_SPECULARENABLE, specularState);
    Core::Graphic::SetRenderState(D3DRS_ZENABLE, zenableState);
}