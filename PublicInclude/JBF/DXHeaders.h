#pragma once

#include<d3d9.h>
#include<d3dx9.h>
#include<dxerr.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dxerr.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif