#include"pch.h"

#include<iostream>
#include<fstream>

#include"JBF/JBFramework.h"
#include"Config.h"

using namespace JBF;

CfgParseINI::CfgParseINI(){ ins_varTable.rehash(1024); }
CfgParseINI::~CfgParseINI(){}

static TCHAR* _strRemoveSpace(TCHAR* str){
    for (;;){
        if (*str == _T(' '))++str;
        else if (*str == _T('\t'))++str;
        else break;
    }
    return str;
}
static size_t _strReadUntilChar(const TCHAR* str, TCHAR ch, size_t len){
    auto i = decltype(len){0};

    for (; i < len; ++i, ++str){
        if (*str == ch)break;
    }
    if (i == len)i = -1;
    
    return i;
}
static bool _strIsNumber(const TCHAR* str, size_t len){
    for (auto i = decltype(len){0}; i < len; ++i, ++str){
        switch (*str){
        case _T('0'):
        case _T('1'):
        case _T('2'):
        case _T('3'):
        case _T('4'):
        case _T('5'):
        case _T('6'):
        case _T('7'):
        case _T('8'):
        case _T('9'):
        case _T('.'):
            continue;
        default:
            return false;
        }
    }
    return true;
}

bool CfgParseINI::Read(const TCHAR* fileName){
    TCHAR rawLine[4096];
    TCHAR tmpStr[4096];

    TCHAR* curLine;
    size_t lineLen;
    size_t tmpLen0, tmpLen1;

    std::basic_string<TCHAR> strName;
    Value valNew;

    std::basic_ifstream<TCHAR, std::char_traits<TCHAR>> ifStream(fileName, std::ios::in);

    if(ifStream){
        for (size_t i = 0; ifStream.getline(rawLine, _countof(rawLine)); ++i){
            curLine = rawLine;
            lineLen = tstrlen(curLine);

            curLine = _strRemoveSpace(curLine);
            if (!tstrncmp(curLine, _T("//"), 2))continue;
            else if (*curLine == _T('\n'))continue;
            else if (*curLine == _T('\r'))continue;
            else if (*curLine == _T('\0'))continue;

            { // read key
                tmpLen0 = _strReadUntilChar(curLine, _T('='), lineLen - (curLine - rawLine));
                if (tmpLen0 == -1){
                    Global::MsgBox(nullptr, _T("Error"), MB_ICONERROR | MB_OK, _T("Error to read on %s.\n Cannot find value.\nLine: %u"), fileName, i + 1);
                    continue;
                }

                tmpLen1 = _strReadUntilChar(curLine, _T(' '), tmpLen0);
                if (tmpLen1 == -1)tmpLen1 = tmpLen0;

                tstrncpy_s(tmpStr, curLine, tmpLen1);
                tmpStr[tmpLen1] = 0;

                curLine += ++tmpLen0;

                strName = tmpStr;
            }

            { // read value
                {
                    valNew.type = Type::UNKNOWN;
                    valNew.interger = 0;
                    valNew.flotingPoint = 0;
                    valNew.string[0] = 0;
                }

                curLine = _strRemoveSpace(curLine);

                tmpLen0 = _strReadUntilChar(curLine, _T(';'), lineLen - (curLine - rawLine));
                if (tmpLen0 == -1){
                    Global::MsgBox(nullptr, _T("Error"), MB_ICONERROR | MB_OK, _T("Error to read on %s.\nCannot find end of line.\nLine: %u"), fileName, i + 1);
                    continue;
                }

                if (*curLine == _T('\"')){ // supposed to string
                    if (curLine[tmpLen0 - 1] != _T('\"')){
                        Global::MsgBox(nullptr, _T("Error"), MB_ICONERROR | MB_OK, _T("Error to read on %s.\nCannot find end of string.\nLine: %u"), fileName, i + 1);
                        continue;
                    }

                    valNew.type = Type::STRING;
                    tstrncpy_s(valNew.string, &curLine[1], tmpLen0 - 2);
                    valNew.string[tmpLen0 - 2] = 0;
                }
                else if(_strIsNumber(curLine, tmpLen0 - 1)){ // supposed to number
                    valNew.type = Type::NUMBER;
                    tstrncpy_s(tmpStr, curLine, tmpLen0 - 1);
                    tmpStr[tmpLen0 - 1] = 0;

                    valNew.interger = _ttoi(tmpStr);
                    valNew.flotingPoint = _ttof(tmpStr);
                }
                else{
                    Global::MsgBox(nullptr, _T("Error"), MB_ICONERROR | MB_OK, _T("Error to read on %s.\nCannot identify value type.\nLine: %u"), fileName, i + 1);
                    continue;
                }

                ins_varTable.emplace(strName, valNew);
            }
        }
        ifStream.close();
    }
    else return false;

    return true;
}

bool CfgParseINI::Get(const std::basic_string<TCHAR>& key, Value* value){
    auto itr = ins_varTable.find(key);

    if (itr != ins_varTable.end()){
        value->type = itr->second.type;

        switch (itr->second.type){
        case Type::NUMBER:
            value->interger = itr->second.interger;
            value->flotingPoint = itr->second.flotingPoint;
            break;

        case Type::STRING:
            tstrcpy_s(value->string, itr->second.string);
            break;
        }
    }
    else return false;

    return true;
}