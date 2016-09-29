#include"pch.h"

#include"Base.h"
#include"Public/Public.h"

BaseMesh::BaseMesh(RES_TYPE type) : Base::DXResource(type), ins_mesh(nullptr), ins_vertDecl(nullptr){}
BaseMesh::~BaseMesh(){}