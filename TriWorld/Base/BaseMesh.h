#pragma once

#include<vector>
#include"JBF/JBFramework.h"

using namespace JBF;

class BaseMesh : public Base::DXResource{
protected:
    ID3DXMesh* ins_mesh;
    IDirect3DVertexDeclaration9* ins_vertDecl;
    std::vector<D3DMATERIAL9> ins_mtrlTable;
    std::vector<Object::ExternalTexture*> ins_texTable;
    DWORD ins_partConut;

protected:
    BaseMesh(RES_TYPE type);
    virtual ~BaseMesh();
};