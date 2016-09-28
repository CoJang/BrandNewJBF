#pragma once

#include<string>
#include<unordered_map>

class CfgParseINI{
public:
    enum Type : unsigned char{
        UNKNOWN,
        NUMBER,
        STRING
    };
    struct Value{
        Type type;
        int interger;
        float flotingPoint;
        TCHAR string[2048];
    };

private:
    std::unordered_map<std::basic_string<TCHAR>, Value> ins_varTable;

public:
    CfgParseINI();
    ~CfgParseINI();

public:
    bool Read(const TCHAR* fileName);

public:
    bool Get(const std::basic_string<TCHAR>& key, Value* value);
};