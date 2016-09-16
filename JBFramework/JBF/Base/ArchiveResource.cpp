#include"pch.h"
#include"JBF/Base/Base.h"

namespace JBF{
    namespace Base{
        ArchiveResourceKey& ArchiveResourceKey::operator=(const ArchiveResourceKey& rhs){
            memcpy_s(this, sizeof(decltype(*this)), &rhs, sizeof(decltype(*this)));
            return *this;
        }
        bool ArchiveResourceKey::operator==(const ArchiveResourceKey& rhs)const{
            return (arc == rhs.arc) && (file == rhs.file);
        }
        size_t ArchiveResourceKey::Hasher::operator()(const ArchiveResourceKey& key)const{
            return (reinterpret_cast<size_t>(key.arc) ^ key.file);
        }
    };
};