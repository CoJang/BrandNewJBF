#pragma once

#include<vector>
#include"JBF/JBFramework.h"

using namespace JBF;

class BaseMesh : Base::DXResource{
private:
    ARCHIVE_HASHSIZE ins_fileName;
    ID3DXMesh* ins_mesh;
    std::vector<D3DMATERIAL9> ins_mtrlTable;

private:
    HRESULT(*ins_validateFunc)(BaseMesh* obj);

public:
    BaseMesh();
    virtual ~BaseMesh();

public:
    bool ParseXFormat(ARCHIVE_HASHSIZE fileName);

public:
    virtual HRESULT Validate();
    virtual void Invalidate();

private:
    static HRESULT ins_fillMeshByXFormat(BaseMesh* obj);
};