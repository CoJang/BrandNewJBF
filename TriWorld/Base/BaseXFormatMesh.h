#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;

class BaseXFormatMesh : public BaseMesh{
protected:
    Global::Archive::Decrypter* ins_arcModel;
    Global::Archive::Decrypter* ins_arcTexture;

    ARCHIVE_HASHSIZE ins_fileName;

protected:
    BaseXFormatMesh();
    virtual ~BaseXFormatMesh();

public:
    virtual HRESULT Validate();
    virtual void Invalidate();
};