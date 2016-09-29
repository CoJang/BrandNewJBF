#pragma once

#pragma once

#include<vector>
#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjDwarf : public BaseXFormatMesh{
private:
    std::vector<Object::ExternalTexture*> ins_texBumpTable;
    std::vector<Object::ExternalTexture*> ins_texSpecTable;

private:
    Matrix ins_mWorld;

private:
    ObjDwarf();
    virtual ~ObjDwarf();

public:
    virtual HRESULT Validate();
    virtual void Invalidate();

public:
    static ObjDwarf* Create(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName);
    void Release();

public:
    void Init(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName);

public:
    void Update(float delta);
    void Draw(Object::Shader* shader);

public:
    INLINE const Vector3* GetPosition()const{ return (const Vector3*)&ins_mWorld._41; }
    INLINE void SetPosition(const Vector3* pos){ *(Vector3*)&ins_mWorld._41 = *pos; }

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
    template<class T> friend void Global::Alloc::DeleteCustomAligned(T*);
};