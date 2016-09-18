#pragma once

#include<malloc.h>
#include<utility>

#include"JBF/Debug/Debug.h"

namespace JBF{
    namespace Global{
        namespace Alloc{
            template<class T, typename... ARGS> extern T* NewCustomAligned(size_t alignedSize, ARGS&&... args){
                auto _new = (T*)_aligned_malloc(sizeof T, alignedSize);

                ASSERT(_new != nullptr, _T("Failed to allocate on memory."));
                if (_new)new (_new)T(std::forward<ARGS>(args)...);
                else throw std::bad_alloc();

                return _new;
            }

            template<class T> extern void DeleteCustomAligned(T* obj){
                obj->~T();
                _aligned_free(obj);
            }
        };
    };
};