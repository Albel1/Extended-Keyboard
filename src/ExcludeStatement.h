#pragma once
#include <string>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <codecvt>

#include <iostream>
class ExcludeStatement{
public:
    ExcludeStatement(std::string groupName, std::unordered_set<std::wstring>* excludedProgramNames,int lineNumber)
        :groupName(groupName),excludedProgramNames(excludedProgramNames),lineNumber(lineNumber){

    }
    void excludeProgram(std::wstring programToExclude){
        std::wcout << L"program to exclude: " << programToExclude << std::endl;
        std::cout << programToExclude.size() << std::endl;
        if (excludedProgramNames->contains(programToExclude)){
            std::cout << "returning" << std::endl;
            return;
        }
        excludedProgramNames->emplace(programToExclude);
        modifyFile();
    }
    void unexcludeProgram(std::wstring programToUnexclude){
        if (!excludedProgramNames->contains(programToUnexclude)) return;
        excludedProgramNames->erase(programToUnexclude);
        modifyFile();
    }
private:
    void modifyFile(){
        std::vector<std::wstring> lines;
        std::wstring line;
        std::wifstream macroFileInput("macros.txt");
        if (!macroFileInput.is_open()){
            throw std::runtime_error("macros.txt not found");
        }
        macroFileInput.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        while(getline(macroFileInput,line)){
            lines.emplace_back(line);
        }
        macroFileInput.close();
        lines[lineNumber] = L"EXCLUDE FROM " + excludededProgramsString();
        std::wofstream macroFileOutput("macros.txt");
        macroFileOutput.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        for (std::wstring& line : lines){
            macroFileOutput << line << std::endl;
        }
        macroFileOutput.close();
    }
    std::wstring excludededProgramsString(){
        std::wstring s = L"[";
        int i = 0;
        for (const std::wstring& excludedProgramName : *excludedProgramNames){
            s += L"\"" + excludedProgramName + L"\"";
            if (i < excludedProgramNames->size()-1){
                s += L", ";
            }
            i++;
        }
        s += L"]";
        return s;
    }
    int lineNumber;
    std::string groupName;
    std::unordered_set<std::wstring>* excludedProgramNames;
};
