#include"pch.h"

#include<cstdio>

#include"JBF/Definitions.h"

int (WINAPIV * __vsnprintf)(char*, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * __vsnwprintf)(wchar_t*, size_t, const wchar_t*, va_list) = _vsnwprintf;