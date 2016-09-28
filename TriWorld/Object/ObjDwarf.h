#pragma once

#pragma once

#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjDwarf : public BaseXFormatMesh{
private:
    Matrix ins_mWorld;

private:
    ObjDwarf();
    virtual ~ObjDwarf();

public:
    static ObjDwarf* Create(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName);
    void Release();

public:
    void Init(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName);

public:
    void Update(float delta);
    void Draw();

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
    template<class T> friend void Global::Alloc::DeleteCustomAligned(T*);
};