#pragma once

#include"JBF/Definitions.h"

namespace JBF{
    namespace Global{
        namespace Hash{
            template<typename HASHSIZE, typename CHARSIZE> extern INLINE HASHSIZE X65599Generator(const CHARSIZE* str, size_t len){
                HASHSIZE h = 0;

                for (auto i = decltype(len){0}; i < len; ++i){
                    h *= 65599;
                    h += ((*str == _T('\\')) ? _T('/') : ((*str >= _T('a') && *str <= _T('z')) ? (*str - 32) : (*str)));
                    ++str;
                }

                h ^= h >> 16;

                return h;
            }

            template<typename T> struct HashedKeyHasher{
                size_t operator()(const T& key)const{ return (size_t)(key); }
            };
            template<typename T> struct PointerKeyHasher{
                size_t operator()(const T& key)const{ return (size_t)(key) / sizeof(T); }
            };
        };
    };
};