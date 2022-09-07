#pragma once
#include <fstream>
#include <codecvt>
class MacroFileWriter{
public:
    MacroFileWriter(){

    }
    static std::wofstream macroFile;
};
std::wofstream MacroFileWriter::macroFile("macros.txt");
//static void imbue(){
//    MacroFileWriter::macroFile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
//}
