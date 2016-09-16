#pragma once

#include<unordered_map>

#include"JBF/Definitions.h"
#include"JBF/Global/Archive.h"

namespace JBF{
    namespace Base{
        struct ArchiveResourceKey{
            Global::Archive::Decrypter* arc;
            ARCHIVE_HASHSIZE file;

            ArchiveResourceKey& operator=(const ArchiveResourceKey& rhs);
            bool operator==(const ArchiveResourceKey& rhs)const;
            struct Hasher{ size_t operator()(const ArchiveResourceKey& key)const; };
        };

        template<typename T> class ArchiveResource{
        protected:
            static std::unordered_map<ArchiveResourceKey, std::pair<T*, DWORD>, ArchiveResourceKey::Hasher> ins_table;
            ArchiveResourceKey ins_file;

        protected:
            template<typename OBJ> static OBJ* ins_read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file, bool(*initFunc)(OBJ* obj)){
                ArchiveResourceKey key = { arc, file };
                auto itr = ins_table.find(key);
                OBJ* _new;

                if (itr == ins_table.end()){
                    _new = new OBJ();
                    _new->ins_file = key;

                    if (initFunc){
                        if (!initFunc(_new)){
                            delete _new;
                            return nullptr;
                        }
                    }

                    ins_table.emplace(key, std::make_pair(_new, (DWORD)0));
                }
                else{
                    _new = itr->second.first;
                    ++itr->second.second;
                }

                return _new;
            }
        public:
            void Release(){
                auto itr = ins_table.find(ins_file);

                if (itr == ins_table.end())return;

                if (itr->second.second)--itr->second.second;
                else{
                    delete itr->second.first;
                    ins_table.erase(itr);
                }
            }

        public:
            static void InitTable(){
                ins_table.rehash(1 << 20);
            }
            static void ReleaseTable(){
                for (auto i = ins_table.begin(), e = ins_table.end(); i != e; ++i){
                    delete i->second.first;
                }
            }
        };

        template<typename T> std::unordered_map<ArchiveResourceKey, std::pair<T*, DWORD>, ArchiveResourceKey::Hasher> ArchiveResource<T>::ins_table;
    };
};